c
c-----------------------------------------------------------------------
c subroutine: i d e f i r
c
c     design of linear-phase fir filters in direct form with minimum
c     coefficient wordlength. possible filter types include lowpass
c     highpass,symmetrical bandpass and bandstop filters (with center
c     frequency at omega = 0.25, where omega = 0.5 is half the sampling
c     frequency), and hilbert transformers.
c     the design is carried out in the following way - -
c     problem transformation to an equivalent lowpass problem;
c     filter length,wordlength, and rounding error estimation;
c     lowpass design with appropriately reduced tolerances;
c     coefficient rounding, tolerance control; iteration if necessary;
c     coefficient transformation back to the desired filter type.
c
c  i n p u t
c     i n p u t  p a r a m e t e r s = tolerance scheme with
c     del1...passband tolerance delta 1;
c     del2...stopband tolerance delta 2;
c     omgp1..first passband edge frequency;
c     omgs1..first stopband edge frequency;
c     omgp2..second passband edge frequency;
c     omgs2..second stopband edge frequency.
c     normalized frequency omg = f / sampling rate.
c     in lowpass and highpass cases,the last two parameters are zero.
c     in bandfilter cases,the last two parameters have the values
c                omgp(s)2 = 0.5 - omgp(s)1
c     in hilbert cases, del2 = del1 is valid;omgp1 is the hilbert band
c     edge frequency; the last t h r e e  parameters are zero.
c
c  o u t p u t
c     o u t p u t  p a r a m e t e r s
c     del1,del2,omgp1,omgs1...tolerance scheme of the equivalent lowpass
c     problem; the original tolerance scheme is replaced .
c     ityp...filter type information- ityp =0..lowpass;
c                                           1..highpass;
c                                           2..bandpass;
c                                           3..bandstop;
c                                           4..hilbert-transformer;
c     nf00...filter length according to tolerances and error estimation;
c     nf.....filter length actually applied;
c     iwcmin...estimated min.coeff.wordlength (according to max.quanti-
c              zation stepsize + sign bit);
c     iwc......wordlength used for the design (in the same sense);
c     iwact....actual wordlength (sign + necessary bits behind the dual
c              point (without leading zeros) for the largest abs.value);
c     del11,del21...reduced tolerances for the equivalent lowpass design
c     emax1,2..tolerances (i.e. max. errors) of the equivalent lowpass
c              with rounded coefficients;
c     h0.......coefficients before rounding;
c     rh0......rounded coefficients;
c     itera....number of design iterations;
c     ierr.....error parameter -
c              ierr= 1...no error;
c                    2...inconsistent specifications;
c                    3...max. allowed filter order exceeded;
c                    4...max. allowed number of iterations exceeded
c-----------------------------------------------------------------------
c
      subroutine idefir(del1, del2, omgp1, omgs1, omgp2, omgs2, ityp,
     *    nf00, nf, iwcmin, iwc, iwact, del11, del21, emax1, emax2, h0,
     *    rh0, itera, ierr)
c
c common-definitions have to be inserted here according to the sub-
c routine 'design' called by this subroutine.
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      common /cheb/ n, ngrid, kgrid, kfp, kfs, ifr(131)
      common /chek/ nl, ng, ka, kb
c
      dimension h0(256), rh0(128)
      ierr = 1
      small = 2.*r1mach(3)
c
c the following parameters concern the max. allowed number of design
c iterations (itmax), the minimum usage of the tolerances (proz),the
c reduction (cr) and increase (ci) factors for the tolerance change.
c
      itmax = 20
      proz = 0.9
      cr = 0.75
      ci = 0.25
c
c transformation to lowpass problem.
c
      call typtra(del1, del2, omgp1, omgs1, omgp2, omgs2, ityp, ierr)
c
c ierr = 2 in the case of inconsistent specifications.
c
      if (ierr.eq.2) return
c
      delomg = omgs1 - omgp1
c
c checking the filter symmetry.
c
      om = abs(omgp1+omgs1-.5)
      isym = 0
      if ((del1.eq.del2) .and. (om.lt.small)) isym = 1
      itera = 0
      isolut = 0
c
c determination of minimum coefficient wordlength.
c
      call wcmin(del1, del2, delomg, iwcmin)
      iwc = iwcmin
c
c o u t e r  d e s i g n  i t e r a t i o n  l o o p
c
  10  continue
      del11 = del1
      del21 = del2
c
c i n n e r  d e s i g n  i t e r a t i o n  l o o p
c
  20  continue
c
c stop after maximum number of iterations.
c
      if (itera.lt.itmax) go to 30
      ierr = 4
      go to 80
  30  continue
c
c determination of filter length.
c
      call length(del11, del21, delomg, nf)
      nn = (nf/2)*2
      if (nn.eq.nf) nf = nf + 1
c
c ierr = 3 , if the max. allowed filter order is exceeded.
c
      if (nf.le.128) go to 40
      if ((ityp.ne.2) .and. (ityp.ne.3) .and. (nf.le.256)) go to 40
      ierr = 3
      return
  40  continue
c
c expected efficient rounding error.
c
      if (isym.eq.1) nf = ((nf+2)/4)*2
      call efferr(del11, nf, iwc, s1)
      call efferr(del21, nf, iwc, s2)
      if (isym.eq.1) nf = 2*nf - 1
c
c comparison of estimated total errors with tolerances.
c
      f1 = del11 + s1
      dd1 = f1 - del1
      f2 = del21 + s2
      dd2 = f2 - del2
      if ((dd1.gt.small).or.(dd2.gt.small)) go to 70
      itera = itera + 1
      if (itera.eq.1) nf0 = nf
      if (itera.eq.1) nf00 = nf0
c
c equivalent lowpass design for reduced tolerances.
c
      n1 = (nf+1)/2
      n = n1
      call design(del11, del21, omgp1, omgs1, h0, nf)
c
c coefficient rounding to (iwc-1) bits behind the dual point.
c
      q = 2.**(1-iwc)
      do 50 i=1,n1
        hr = h0(i)/q
        rh0(i) = q*aint(hr)
  50  continue
c
c determination of real max. errors.
c
      call maxerr(rh0, n1, nf, 0., omgp1, emax1)
      call maxerr(rh0, n1, nf, omgs1, .5, emax2)
c
c comparison with tolerances.
c
      dd1 = emax1 - del1
      dd2 = emax2 - del2
      if ((dd1.gt.small).or.(dd2.gt.small)) go to 60
      isolut = isolut + 1
c
c checking for sufficient usage of tolerances.
c
      if ((dd1/del1).ge.(proz-1.)) dd1 = 0.
      if ((dd2/del2).ge.(proz-1.)) dd2 = 0.
      if ((dd1.eq.0.) .and. (dd2.eq.0.)) go to 80
      if (nf.le.nf0) go to 80
      if (isolut.eq.1) nf0 = nf
c
c tolerance change.
c
  60  continue
      if (dd1.le.0.) del11 = del11 - ci*dd1
      if (dd2.le.0.) del21 = del21 - ci*dd2
  70  continue
      if (dd1.gt.0.) del11 = del11 - cr*dd1
      if (dd2.gt.0.) del21 = del21 - cr*dd2
      if (isym.eq.1) del11 = amin1(del11,del21)
      if (isym.eq.1) del21 = del11
c
c check for realizability.
c
      if ((del11.gt.0.) .and. (del21.gt.0.)) go to 20
c
c e n d  o f  i n n e r  d e s i g n  i t e r a t i o n  l o o p .
c
c
c increase of wordlength in case of unrealizability.
c
      iwc = iwc + 1
      go to 10
c
c e n d  o f  o u t e r  d e s i g n  i t e r a t i o n  l o o p .
c
c
c transformation to desired filter type.
c
  80  continue
      if (ityp.eq.0) go to 90
      call bactra(ityp, h0, rh0, nf)
      n1 = (nf+1)/2
  90  continue
c
c determination of actual coefficient wordlength.
c
      call wcact(rh0, n1, iwc, iwact)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: typtra
c transformation to equivalent lowpass;specifications test.
c-----------------------------------------------------------------------
c
      subroutine typtra(del1, del2, omgp1, omgs1, omgp2, omgs2, ityp,
     *    ierr)
c
c     transformation to equivalent lowpass problem.
c     identification of filter types- ityp = 0..lowpass)
c                                     ityp = 1..highpass)
c                                     ityp = 2..bandpass)
c                                     ityp = 3..bandstop)
c                                     ityp = 4..hilbert-transformer
c
c l o w p a s s
c
      if ((omgp2.ne.0.) .or. (omgs2.ne.0.)) go to 20
      if (omgp1.gt.omgs1) go to 10
      ityp = 0
      go to 60
c
c h i g h p a s s
c
  10  continue
      if (omgs1.eq.0.) go to 40
      ityp = 1
      omgp1 = .5 - omgp1
      omgs1 = .5 - omgs1
      go to 60
  20  continue
c
c checking for inconsistent specifications.
c set up a machine constant.
c
      epsmac = 2.*r1mach(3)
      dop = abs(omgp1+omgp2-.5)
      dos = abs(omgs1+omgs2-.5)
      do = amax1(dop,dos)
      if (do.ge.epsmac) go to 50
c
c b a n d p a s s
c
      if (omgp1.lt.omgs1) go to 30
      ityp = 2
      omgp1 = .5 - 2.*omgp1
      omgs1 = .5 - 2.*omgs1
      go to 60
c
c b a n d s t o p
c
  30  continue
      ityp = 3
      omgp1 = 2.*omgp1
      omgs1 = 2.*omgs1
      go to 60
c
c h i l b e r t - t r a n s f o r m e r
c
  40  continue
c
c checking for inconsistent specifications.
c
      if ((del1.ne.del2) .and. (del2.ne.0.)) go to 50
      ityp = 4
      omgs1 = .25 + omgp1
      omgp1 = .25 - omgp1
      del1 = .5*del1
      del2 = del1
      go to 60
c
c error - inconsistent specifications.
c
  50  ierr = 2
c
  60  return
      end
c
c-----------------------------------------------------------------------
c subroutine: bactra
c transformation of the designed lowpass to the desired filter type.
c-----------------------------------------------------------------------
c
      subroutine bactra(ityp, h0, rh0, nf)
c
      dimension h0(256), rh0(128)
c
      n1 = (nf+1)/2
      if (ityp-3) 10, 30, 60
  10  continue
c
c h i g h p a s s  and b a n d p a s s
c
      n = n1 - 1
      do 20 i=1,n,2
        j = n1 - i
        h0(j) = -h0(j)
        rh0(j) = -rh0(j)
  20  continue
      if (ityp.eq.1) go to 80
  30  continue
c
c b a n d p a s s  and  b a n d s t o p
c
      n11 = n1
      n1 = nf
      nf = 2*nf - 1
      nn = 2*n11
      i = n1
      j = n11
      if (nn-n1) 40, 40, 50
  40  continue
c
c lowpass filter length even.
c
      h0(i) = 0.
      rh0(i) = 0.
      i = i - 1
  50  continue
c
c lowpass filter length even or odd.
c
      h0(i) = h0(j)
      h0(i-1) = 0.
      rh0(i) = rh0(j)
      rh0(i-1) = 0.
      i = i - 2
      j = j - 1
      if (j-1) 80, 80, 50
  60  continue
c
c h i l b e r t - t r a n s f o r m e r
c
      i = n1
      h0(i) = 0.
      rh0(i) = 0.
      v = -2.
      i = i - 1
  70  continue
      v = -v
      h0(i) = v*h0(i)
      rh0(i) = v*rh0(i)
      i = i - 2
      if (i) 80, 80, 70
  80  return
      end
c
c-----------------------------------------------------------------------
c subroutine: wcmin
c minimum coefficient wordlength estimation by an empirical formula.
c-----------------------------------------------------------------------
c
      subroutine wcmin(d1, d2, domg, iw)
c
      ald = alog10(d1)
      als = alog10(d2)
      alo = alog10(domg*360.)
      w = 7.22116 - 0.589268*ald
      w = w + (0.112354*ald-2.328539)*alo
      w = w - 3.559051*als
      iw = ifix(w)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: length
c filter length estimation by an empirical formula.
c-----------------------------------------------------------------------
c
      subroutine length(d1, d2, domg, nf)
c
      dl1 = alog10(d1)
      dl2 = alog10(d2)
      dq1 = dl1*dl1
      d = .005309*dq1 + .07114*dl1 - .4761
      d = d*dl2
      d = d - .00266*dq1 - .5941*dl1 - .4278
      d = 2. + d/domg
      nf = ifix(d)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: efferr
c iterative calculation of the approximate statistical error bound s
c for a given tolerance d,filter length nf,coeff.wordlength iw.
c-----------------------------------------------------------------------
c
      subroutine efferr(d, nf, iw, s)
c
      qh = 2.**(-iw)
      call sstat(nf, 0., sq)
      alpha = d/(sq*qh)
  10  continue
      call sstat(nf, alpha, sq)
      alpha1 = d/(sq*qh)
      dalpha = alpha1 - alpha
      if (dalpha-.05) 30, 30, 20
  20  continue
      alpha = alpha1
      go to 10
  30  continue
      s = qh*sq
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: sstat
c evaluation of the approximate statistical error formula.
c-----------------------------------------------------------------------
c
      subroutine sstat(nf, alpha, sq)
c
      pi = 4.*atan(1.0)
      piq = pi*pi
c
c weighting function m(alpha).
c
      am = 1./(1.+sqrt(alpha))
c
c error bound.
c
      anz = float(nf-1)
      sq = (anz/pi+.5)*am
      x = (4.-24.*am/piq)*anz
      x = x + 4. - 3.*am
      x = sqrt(x*am/3.)
      sq = sq + x
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: maxerr
c search for the max.abs.error of a linear-phase fir filter.
c-----------------------------------------------------------------------
c
      subroutine maxerr(rh0, n1, nf, omga, omge, emax)
c
      dimension rh0(128)
      emax = 0.
      omg = omga
      ia = 1
      istg = 1
      dom1 = 1./(4.*float(nf))
      dom2 = dom1/2.
      dom3 = dom2/2.
      dfr = .05
      dfrh = .025
      call error(rh0, n1, nf, omg, e)
      bf1 = abs(e)
      if (bf1.gt.emax) emax = bf1
      delomg = dom3
      omg = omg + delomg
  10  continue
      bf2 = bf1
  20  continue
      call error(rh0, n1, nf, omg, e)
      bf1 = abs(e)
      if (bf1.gt.emax) emax = bf1
      if (ia) 30, 110, 30
  30  continue
      delfr = 2.*(bf1-bf2)/(bf1+bf2)
      if (istg) 90, 90, 40
  40  continue
      if (delfr) 80, 80, 50
  50  continue
      istg = 1
      delomg = dom2
      if (delfr.lt.dfrh) delomg = dom3
      if (delfr.gt.dfr) delomg = dom1
      omg = omg + delomg
  60  continue
      if (omg-omge) 10, 70, 70
  70  continue
      omg = omge
      ia = 0
      go to 10
  80  continue
      istg = -1
      if (delomg.eq.dom1) delomg = -3.*dom3
      if (delomg.eq.dom2) delomg = -dom3
      if (delomg.eq.dom3) delomg = dom1
      omg = omg + delomg
      if (delomg) 20, 60, 60
  90  continue
      if (delfr) 100, 100, 50
 100  continue
      delomg = dom1
      omg = omg + delomg
      go to 60
 110  return
      end
c
c-----------------------------------------------------------------------
c subroutine: error
c error e of a linear-phase fir lowpass transfer function with
c respect to the ideal lowpass.
c-----------------------------------------------------------------------
c
      subroutine error(rh0, n1, nf, omg, e)
c
      dimension rh0(128)
c
      pi = 4.*atan(1.0)
      pi2 = pi*2.
      nn = 2*n1
      if (nf-nn) 10, 30, 30
  10  continue
c
c length nf odd.
c
      h = rh0(n1)
      n = n1 - 1
      do 20 i=1,n
        annu = float(n1-i)
        h = h + 2.*rh0(i)*cos(annu*omg*pi2)
  20  continue
      go to 50
  30  continue
c
c length nf even.
c
      h = 0.
      do 40 i=1,n1
        annu = float(n1-i) + .5
        h = h + rh0(i)*cos(annu*omg*pi2)
  40  continue
      h = h*2.
  50  continue
c
c error value e.
c
      e = h
      e1 = h - 1.
      ea = abs(e)
      ea1 = abs(e1)
      if (ea1.lt.ea) e = e1
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: wcact
c determination of actual wordlength.
c-----------------------------------------------------------------------
c
      subroutine wcact(rh0, n1, iwc, iwact)
c
      dimension rh0(128)
      hmax = abs(rh0(1))
      do 10 i=2,n1
        h = abs(rh0(i))
        hmax = amax1(hmax,h)
  10  continue
      l = -2
  20  continue
      l = l + 1
      if (hmax-1.) 30, 40, 40
  30  continue
      hmax = hmax*2.
      go to 20
  40  continue
      iwact = iwc - l
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: design
c auxiliary routine calling the odd-length lowpass design routine
c 'tomsb2'.
c-----------------------------------------------------------------------
c
      subroutine design(del11, del21, omgp1, omgs1, h0, nf)
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      common /cheb/ n, ngrid, kgrid, kfp, kfs, ifr(131)
      common /chek/ nl, ng, ka, kb
      dimension h0(256)
c
c auxiliary variables for 'tomsb2'.
c
      fp = omgp1
      fs = omgs1
      call tomsb2(del11, del21, fp, fs, h0, nf)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: tomsb2
c     subroutine according to "program tom" with  the
c     exchange algorithm for designing low pass finite length impulse
c     response digital filters with linear phase.
c
c     j.h. mcclellan and t.w. parks---rice university---october 26, 1971
c
c     the filter length (nf), the passband cutoff frequency (fp),
c     the stopband frequency (fs), and the weighting factor (aa) are
c     inputs to the algorithm.  then the stopband deviation (ds) is
c     minimized and thus the passband deviation (dp=aa*ds) is also
c     minimized. the program seks to determine the best location of
c     the peaks of the error curve in order to minimize the deviations.
c
c     reference--parks,t.w. and j.h. mcclellan,*chebychev approximation
c     of non-recursive digital filters with linear phase*, ieee trans.
c     on circuit theory, vol. ct-19, march, 1972.
c
c     special parameters for change of fs, if stopband ripple is
c     specified
c     d1d   =  stopband ripple d2
c     step  =  initial stepsize for changing fs& step decreases with
c              exp.& if solution  far from optimum try 0.02
c     xac   =  accuracy of d2
c              1000. means accuracy to 0.1 percent
c              100.  means accuracy to 1 percent etc.
c     if xac = d1d  no action is taken and fs remains unchanged
c
c        dimension of x, ad, y, ifr, fopt = n*/2+3 where n* is
c        maximum length of n
c        dimension of af = 16(n*/2+3)+1
c-----------------------------------------------------------------------
c
      subroutine tomsb2(del1, del2, fp, fs, h0, nf)
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      common /cheb/ n, ngrid, kgrid, kfp, kfs, ifr(131)
      common /chek/ nl, ng, ka, kb
      dimension h0(256)
c
      iwchn = i1mach(2)
      do 5 i=1,131
        x(i) = 0.
   5  continue
c
c initialization of the grid of 16n points.  the constant lgrid
c can be changed if a different grid density is desired.
c
      lgrid = 16
      xac = del2
      aa = del1/del2
      d1d = del2
      step = 0.02
      ich = 1
      kiter = 0
      d1c = d1d/xac
  10  continue
      pi2 = 8.0*atan(1.0)
      kcount = 0
      tw = fs - fp
      ngrid = lgrid*(n-1)
      ngrid = ngrid + lgrid/2
      kgrid = 2
      delf = 0.5/float(ngrid)
      af(1) = 0.0
      do 20 j=1,ngrid
        af(j+1) = float(j)*delf
  20  continue
      kfp = fp/delf - 1.
      kfs = fs/delf
  30  continue
      if (fp.lt.af(kfp+1)) go to 40
      kfp = kfp + 1
      go to 30
  40  continue
      if (fs.le.af(kfs)) go to 50
      kfs = kfs + 1
      go to 40
  50  continue
      af(kfp) = fp
      af(kfs) = fs
      ngrid = ngrid + 1
      do 60 j=1,ngrid
        af(j) = cos(pi2*af(j))
  60  continue
c
c initial guess for the n+1 optimal frequencies
c
      kp = (kfp-1)/2
      kp = 2*kp + 1
      ks = kfs/2
      ks = 2*ks + 1
      xpp = kp - 1
      xss = ngrid - ks
      xaa = xpp + xss
      pt = xaa/float(n-1)
      if (pt.gt.3.) go to 70
      write (iwchn,9999)
9999  format (20h error in input data)
      go to 560
  70  kr = xpp/pt
      if (kr.eq.0) kr = kr + 1
      pt = xpp/float(kr)
      ifr(1) = 1
      do 80 j=1,kr
        xtw = float(j)*pt
        ntw = ifix(xtw) + 1
        ntw = ntw/2
        ifr(j+1) = 2*ntw + 1
  80  continue
      kr = kr + 1
      ifr(kr) = kp
      ifr(kr+1) = ks
      ntw = n - kr
      if (ntw.ge.0) go to 90
      write (iwchn,9999)
      go to 560
  90  if (ntw.eq.0) go to 110
      pt = xss/float(ntw)
      do 100 j=1,ntw
        xtw = float(j)*pt + float(ks) - 1.0
        nb = xtw
        nb = nb/2
        k111 = kr + 1 + j
        ifr(k111) = 2*nb - 1
 100  continue
 110  continue
c
c main calculation routine
c calculate rho on the optimal set
c the first 4 passes are made on a grid of 8n points then the
c finer grid of 16n points is used.
c
 120  continue
      n111 = n + 1
      do 130 j=1,n111
        j111 = ifr(j)
        x(j) = af(j111)
 130  continue
      m = (n-1)/15 + 1
      m = (n-1)/20 + 1
      n111 = n + 1
      do 140 j=1,n111
        ad(j) = d(j,n+1,m)
 140  continue
      r1 = 0.0
      do 150 j=1,kr
        r1 = r1 + ad(j)
 150  continue
      r2 = 0.0
      k = 1
      do 160 j=1,kr
        r2 = r2 + ad(j)*aa*float(k)
        k = -k
 160  continue
      k111 = kr + 1
      n111 = n + 1
      do 170 j=k111,n111
        r2 = r2 + float(k)*ad(j)
        k = -k
 170  continue
      rho = r1/r2
      nu = 1
      if (rho.gt.0.0) nu = -1
      rho = abs(rho)
      k = nu
      xray = x(kr+1)
      do 180 j=1,kr
        ad(j) = ad(j)*(xray-x(j))
        y(j) = 1.0 + aa*float(k)*rho
        k = -k
 180  continue
      k = -k
      k111 = kr + 1
      do 190 j=k111,n
        x(j) = x(j+1)
        ad(j) = ad(j+1)*(xray-x(j))
        y(j) = float(k)*rho
        k = -k
 190  continue
      y(n+1) = float(k)*rho
      nl = 0
      ng = 0
      test = gee(ifr(kr+1))
      ro = abs(test-rho)/rho
      if (ro.lt.0.1 .or. kcount.lt.4) go to 200
      write (iwchn,9998)
9998  format (20h interpolation error)
      go to 560
 200  continue
c
c search for the new extremal frequencies
c
      ntot = 0
      npt = ifr(1)
      ng = ifr(2)
      ka = 1
      kb = 1
      ksign = nu
      xt = y(1)
      nfir = ifr(1)
      nold = ifr(n+1)
      if (kr.eq.1) go to 330
      if (npt-1) 210, 260, 210
 210  zt = gee(npt-kgrid)
      if (float(ksign)*(zt-xt)) 260, 260, 220
 220  kpt = npt - kgrid
 230  kpt = kpt - kgrid
      xt = zt
      if (kpt-1) 250, 240, 240
 240  zt = gee(kpt)
      if (float(ksign)*(zt-xt)) 250, 250, 230
 250  ifr(1) = kpt + kgrid
      ntot = ntot + 1
      ksign = -ksign
      go to 310
 260  zt = gee(npt+kgrid)
      if (float(ksign)*(zt-xt)) 300, 300, 270
 270  kpt = npt
 280  kpt = kpt + kgrid
      xt = zt
      zt = gee(kpt+kgrid)
      if (float(ksign)*(zt-xt)) 290, 290, 280
 290  ifr(1) = kpt
      ntot = ntot + 1
 300  ksign = -ksign
 310  xfir = abs(1.0-xt)/aa
      ka = 2
 320  if (ka.gt.(n+1)) go to 420
      nl = npt
      ng = ngrid + 1
      if (ka.le.n) ng = ifr(ka+1)
      npt = ifr(ka)
      xt = y(ka)*float(kb)
      if (ka.eq.kr) go to 330
      if (ka.eq.(kr+1)) go to 340
      go to 350
 330  if ((kfp-npt).lt.kgrid) go to 410
      npt = kfp
      ntot = 1
      go to 410
 340  kb = -1
      if ((npt-kfs).lt.kgrid) go to 410
      npt = kfs
      ntot = 1
      go to 410
 350  kpt = npt
      zt = gee(kpt-kgrid)
      if (float(ksign)*(zt-xt)) 380, 380, 360
 360  xt = zt
      kpt = kpt - kgrid
      zt = gee(kpt-kgrid)
      if (float(ksign)*(zt-xt)) 370, 370, 360
 370  ifr(ka) = kpt
      ksign = -ksign
      ntot = ntot + 1
      ka = ka + 1
      go to 320
 380  if ((kpt+kgrid).gt.ngrid) go to 410
      zt = gee(kpt+kgrid)
      if (float(ksign)*(zt-xt)) 410, 410, 390
 390  xt = zt
      kpt = kpt + kgrid
      if ((kgrid+kpt).gt.ngrid) go to 400
      zt = gee(kpt+kgrid)
      if (float(ksign)*(zt-xt)) 400, 400, 390
 400  ifr(ka) = kpt
      ksign = -ksign
      ntot = ntot + 1
      ka = ka + 1
      go to 320
 410  ifr(ka) = npt
      ksign = -ksign
      ka = ka + 1
      go to 320
 420  continue
      xold = abs(xt)*aa
      nl = 0
      ng = 0
      if (ifr(1).eq.1) go to 450
      if (ifr(n+1).eq.ngrid) go to 430
      write (iwchn,9997)
9997  format (38h error-neither endpoint is an extremum)
      go to 560
 430  if (nfir.eq.1) go to 470
      test = gee(1)
      test = -float(nu)*(test-1.0) - xold
      if (test.le.0.0) go to 470
      ntot = 1
      do 440 j=1,n
        n111 = n + 2 - j
        m111 = n + 1 - j
        ifr(n111) = ifr(m111)
 440  continue
      ifr(1) = 1
      kr = kr + 1
      go to 470
 450  if (ifr(n+1).eq.ngrid) go to 470
      if (nold.eq.ngrid) go to 470
      test = gee(ngrid)
      test = float(ksign)*test - xfir
      if (test.le.0.0) go to 470
      ntot = 1
      do 460 j=1,n
        ifr(j) = ifr(j+1)
 460  continue
      ifr(n+1) = ngrid
      kr = kr - 1
 470  continue
      if (kr.gt.0) go to 480
      write (iwchn,9996)
9996  format (6h error)
      go to 560
 480  continue
      kcount = kcount + 1
      if (kcount.gt.50) go to 500
      if (kcount.eq.4 .and. kgrid.eq.2) go to 490
      if (ntot.ne.0) go to 120
      if (kgrid.eq.2) go to 490
      go to 500
 490  kgrid = 1
      go to 120
 500  continue
      if (abs(rho-d1d).lt.d1c) go to 520
      kiter = kiter + 1
      if (kiter.gt.1000) go to 520
      if (rho.gt.d1d) go to 510
      if (ich.eq.1) step = step/2.
      fs = fs - step
      ich = -1
      go to 10
 510  if (ich.eq.(-1)) step = step/2.
      fs = fs + step
      ich = 1
      go to 10
 520  continue
      krp = n - kr
      r1 = aa*rho
      j = n
 530  continue
      if (x(j)) 540, 550, 540
 540  j = j + 1
      go to 530
 550  x(j) = -1.
      call pref2(h0, nf)
      go to 570
 560  continue
      stop
 570  continue
      return
      end
c
c-----------------------------------------------------------------------
c function: d
c the subroutine d(,,,) calculates the lagrangian interpolation
c coefficients for use in the barycentric interpolation formula
c-----------------------------------------------------------------------
c
      function d(k, n, m)
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      d = 1.0
      q = x(k)
      do 30 l=1,m
        do 20 j=l,n,m
          if (j-k) 10, 20, 10
  10      d = 2.0*d*(x(j)-q)
  20    continue
  30  continue
      d = 1.0/d
      return
      end
c
c-----------------------------------------------------------------------
c function: gee
c the subroutine gee evaluates the frequency response using the
c interpolation coefficients which are calculated in the routine d.
c-----------------------------------------------------------------------
c
      function gee(k)
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      common /cheb/ n, ngrid, kgrid, kfp, kfs, ifr(131)
      common /chek/ nl, ng, ka, kb
      if (k-nl) 10, 40, 10
  10  if (k-ng) 20, 40, 20
  20  p = 0.0
      xf = af(k)
      d = 0.0
      do 30 j=1,n
        c = ad(j)/(xf-x(j))
        d = d + c
        p = p + c*y(j)
  30  continue
      gee = p/d
      return
  40  k111 = ka + kb
      gee = y(k111)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: pref2
c the subroutine pref samples the frequency response at 2**m points
c so that the fft (r.c. singleton) can be called to inverse transform
c this data to obtain the impulse response.
c-----------------------------------------------------------------------
c
      subroutine pref2(a, nf)
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      common /cheb/ n, ngrid, kgrid, kfp, kfs, ifr(131)
      complex z
      dimension a(256), b(256)
      dimension z(256)
      equivalence (b(1),af(257)), (z(1),af(520))
c
c set up a machine constant.
c
      fsh = r1mach(1)
c
      pi2 = 8.0*atan(1.0)
      nx = 2
      mx = 1
  10  if (nx.ge.nf) go to 20
      nx = 2*nx
      mx = mx + 1
      go to 10
  20  nn = nx
      mm = mx
      nx = nx/2
      xnn = nn
      df = 1.0/xnn
      if (ifr(1).eq.1) go to 30
      p = goo(1.0)
      go to 40
  30  p = y(1)
  40  a(1) = p
      if (ifr(n+1).eq.ngrid) go to 50
      p = goo(-1.0)
      go to 60
  50  p = y(n)
  60  a(nx+1) = p
      l = 1
      n111 = nx - 1
      do 110 j=1,n111
        at = df*float(j)
        at = cos(pi2*at)
  70    as = x(l)
        if (at.gt.as) go to 90
        if ((as-at).lt.fsh) go to 80
        l = l + 1
        go to 70
  80    a(j+1) = y(l)
        go to 100
  90    if ((at-as).lt.fsh) go to 80
        a(j+1) = goo(at)
 100    l111 = nn + 1 - j
        a(l111) = a(j+1)
        b(j+1) = 0.0
        if (l.gt.1) l = l - 1
        m111 = nn + 1 - j
        b(m111) = 0.
 110  continue
      b(1) = 0.0
      nx1 = nx + 1
      b(nx1) = 0.0
      do 120 j=1,nn
        z(j) = cmplx(a(j),b(j))
 120  continue
      call bitrev(z, mm)
      call fftft(z, mm)
      do 130 j=1,nx
        b(j) = real(z(j))
 130  continue
      do 140 j=1,n
        jj = n + 1 - j
        a(jj) = b(j)
 140  continue
      return
      end
c
c-----------------------------------------------------------------------
c function: goo
c the subroutine goo is the same as gee except that it is called by
c a frequency value rather than by by a grid index value.
c-----------------------------------------------------------------------
c
      function goo(f)
c
      common /calc/ x(131), af(2096), ad(131), y(131), scale
      common /cheb/ n, ngrid, kgrid, kfp, kfs, ifr(131)
      xf = f
      p = 0.0
      d = 0.0
      do 20 j=1,n
        if (xf.ne.x(j)) go to 10
        goo = y(j)
        go to 30
  10    c = ad(j)/(xf-x(j))
        d = d + c
        p = p + c*y(j)
  20  continue
      goo = p/d
  30  return
      end
c
c-----------------------------------------------------------------------
c subroutine: fftft
c frequency-to-time fft routine; inplace algorithm; complex data in
c array a; input data assumed to be in bitreversed order.
c-----------------------------------------------------------------------
c
      subroutine fftft(a, m)
c
      complex a, u, w, t
      dimension a(1)
      pi = 4.0*atan(1.)
      n = 2**m
      do 30 l=1,m
        le = 2**l
        le1 = le/2
        u = (1.0,0.0)
        w = cmplx(cos(pi/float(le1)),sin(pi/float(le1)))
        do 20 j=1,le1
          do 10 i=j,n,le
            ip = i + le1
            t = a(ip)*u
            a(ip) = a(i) - t
            a(i) = a(i) + t
  10      continue
          u = u*w
  20    continue
  30  continue
      do 40 k=1,n
        a(k) = a(k)/float(n)
  40  continue
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: bitrev
c reordering of 2**m data in complex array a in bitreversed order.
c-----------------------------------------------------------------------
c
      subroutine bitrev(a, m)
c
      dimension a(1)
      complex a, t
c
      n = 2**m
      nv2 = n/2
      nm1 = n - 1
      j = 1
      do 40 i=1,nm1
        if (i.ge.j) go to 10
        t = a(j)
        a(j) = a(i)
        a(i) = t
  10    k = nv2
  20    if (k.ge.j) go to 30
        j = j - k
        k = k/2
        go to 20
  30    j = j + k
  40  continue
      return
      end
