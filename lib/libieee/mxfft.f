c
c-----------------------------------------------------------------------
c block data:  initializes labeled common
c-----------------------------------------------------------------------
c
      block data
c
      common /cstak/ dstak(2500)
c
      double precision dstak
      integer istak(5000)
      integer isize(5)
c
      equivalence (dstak(1),istak(1))
      equivalence (istak(1),lout)
      equivalence (istak(2),lnow)
      equivalence (istak(3),lused)
      equivalence (istak(4),lmax)
      equivalence (istak(5),lbook)
      equivalence (istak(6),isize(1))
c
      data isize(1), isize(2), isize(3), isize(4), isize(5) /1,1,1,2,2/
      data lout, lnow, lused, lmax, lbook /0,10,10,5000,10/
c
      end
c
c-----------------------------------------------------------------------
c subroutine:  fft
c multivariate complex fourier transform, computed in place
c using mixed-radix fast fourier transform algorithm.
c-----------------------------------------------------------------------
c
      subroutine fft(a, b, nseg, n, nspn, isn)
c
c arrays a and b originally hold the real and imaginary
c      components of the data, and return the real and
c      imaginary components of the resulting fourier coefficients.
c multivariate data is indexed according to the fortran
c      array element successor function, without limit
c      on the number of implied multiple subscripts.
c      the subroutine is called once for each variate.
c      the calls for a multivariate transform may be in any order.
c
c n is the dimension of the current variable.
c nspn is the spacing of consecutive data values
c      while indexing the current variable.
c nseg*n*nspn is the total number of complex data values.
c the sign of isn determines the sign of the complex
c      exponential, and the magnitude of isn is normally one.
c      the magnitude of isn determines the indexing increment for a&b.
c
c if fft is called twice, with opposite signs on isn, an
c      identity transformation is done...calls can be in either order.
c      the results are scaled by 1/n when the sign of isn is positive.
c
c a tri-variate transform with a(n1,n2,n3), b(n1,n2,n3)
c is computed by
c        call fft(a,b,n2*n3,n1,1,-1)
c        call fft(a,b,n3,n2,n1,-1)
c        call fft(a,b,1,n3,n1*n2,-1)
c
c a single-variate transform of n complex data values is computed by
c        call fft(a,b,1,n,1,-1)
c
c the data may alternatively be stored in a single complex
c      array a, then the magnitude of isn changed to two to
c      give the correct indexing increment and a(2) used to
c      pass the initial address for the sequence of imaginary
c      values, e.g.
c        call fft(a,a(2),nseg,n,nspn,-2)
c
c array nfac is working storage for factoring n.  the smallest
c      number exceeding the 15 locations provided is 12,754,584.
c
      dimension a(1), b(1), nfac(15)
c
      common /cstak/ dstak(2500)
      double precision dstak
      integer istak(5000)
      real rstak(5000)
c
      equivalence (dstak(1),istak(1))
      equivalence (dstak(1),rstak(1))
c
c determine the factors of n
c
      m = 0
      nf = iabs(n)
      k = nf
      if (nf.eq.1) return
      nspan = iabs(nf*nspn)
      ntot = iabs(nspan*nseg)
      if (isn*ntot.ne.0) go to 20
      ierr = i1mach(4)
      write (ierr,9999) nseg, n, nspn, isn
9999  format (31h error - zero in fft parameters, 4i10)
      return
c
  10  m = m + 1
      nfac(m) = 4
      k = k/16
  20  if (k-(k/16)*16.eq.0) go to 10
      j = 3
      jj = 9
      go to 40
  30  m = m + 1
      nfac(m) = j
      k = k/jj
  40  if (mod(k,jj).eq.0) go to 30
      j = j + 2
      jj = j**2
      if (jj.le.k) go to 40
      if (k.gt.4) go to 50
      kt = m
      nfac(m+1) = k
      if (k.ne.1) m = m + 1
      go to 90
  50  if (k-(k/4)*4.ne.0) go to 60
      m = m + 1
      nfac(m) = 2
      k = k/4
c all square factors out now, but k .ge. 5 still
  60  kt = m
      maxp = max0(kt+kt+2,k-1)
      j = 2
  70  if (mod(k,j).ne.0) go to 80
      m = m + 1
      nfac(m) = j
      k = k/j
  80  j = ((j+1)/2)*2 + 1
      if (j.le.k) go to 70
  90  if (m.le.kt+1) maxp = m + kt + 1
      if (m+kt.gt.15) go to 120
      if (kt.eq.0) go to 110
      j = kt
 100  m = m + 1
      nfac(m) = nfac(j)
      j = j - 1
      if (j.ne.0) go to 100
c
 110  maxf = m - kt
      maxf = nfac(maxf)
      if (kt.gt.0) maxf = max0(nfac(kt),maxf)
      j = istkgt(maxf*4,3)
      jj = j + maxf
      j2 = jj + maxf
      j3 = j2 + maxf
      k = istkgt(maxp,2)
      call fftmx(a, b, ntot, nf, nspan, isn, m, kt, rstak(j),
     *    rstak(jj), rstak(j2), rstak(j3), istak(k), nfac)
      call istkrl(2)
      return
c
 120  ierr = i1mach(4)
      write (ierr,9998) n
9998  format (50h error - fft parameter n has more than 15 factors-,
     *    i20)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine:  fftmx
c called by subroutine 'fft' to compute mixed-radix fourier transform
c-----------------------------------------------------------------------
c
      subroutine fftmx(a, b, ntot, n, nspan, isn, m, kt, at, ck, bt,
     *    sk, np, nfac)
c
      dimension a(1), b(1), at(1), ck(1), bt(1), sk(1), np(1), nfac(1)
c
      inc = iabs(isn)
      nt = inc*ntot
      ks = inc*nspan
      rad = atan(1.0)
      s72 = rad/0.625
      c72 = cos(s72)
      s72 = sin(s72)
      s120 = sqrt(0.75)
      if (isn.gt.0) go to 10
      s72 = -s72
      s120 = -s120
      rad = -rad
      go to 30
c
c scale by 1/n for isn .gt. 0
c
  10  ak = 1.0/float(n)
      do 20 j=1,nt,inc
        a(j) = a(j)*ak
        b(j) = b(j)*ak
  20  continue
c
  30  kspan = ks
      nn = nt - inc
      jc = ks/n
c
c sin, cos values are re-initialized each lim steps
c
      lim = 32
      klim = lim*jc
      i = 0
      jf = 0
      maxf = m - kt
      maxf = nfac(maxf)
      if (kt.gt.0) maxf = max0(nfac(kt),maxf)
c
c compute fourier transform
c
  40  dr = 8.0*float(jc)/float(kspan)
      cd = 2.0*sin(0.5*dr*rad)**2
      sd = sin(dr*rad)
      kk = 1
      i = i + 1
      if (nfac(i).ne.2) go to 110
c
c transform for factor of 2 (including rotation factor)
c
      kspan = kspan/2
      k1 = kspan + 2
  50  k2 = kk + kspan
      ak = a(k2)
      bk = b(k2)
      a(k2) = a(kk) - ak
      b(k2) = b(kk) - bk
      a(kk) = a(kk) + ak
      b(kk) = b(kk) + bk
      kk = k2 + kspan
      if (kk.le.nn) go to 50
      kk = kk - nn
      if (kk.le.jc) go to 50
      if (kk.gt.kspan) go to 350
  60  c1 = 1.0 - cd
      s1 = sd
      mm = min0(k1/2,klim)
      go to 80
  70  ak = c1 - (cd*c1+sd*s1)
      s1 = (sd*c1-cd*s1) + s1
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, substitute
c c1=ak
c
c      c1 = 0.5/(ak**2+s1**2) + 0.5
c      s1 = c1*s1
c      c1 = c1*ak
      c1 = ak
  80  k2 = kk + kspan
      ak = a(kk) - a(k2)
      bk = b(kk) - b(k2)
      a(kk) = a(kk) + a(k2)
      b(kk) = b(kk) + b(k2)
      a(k2) = c1*ak - s1*bk
      b(k2) = s1*ak + c1*bk
      kk = k2 + kspan
      if (kk.lt.nt) go to 80
      k2 = kk - nt
      c1 = -c1
      kk = k1 - k2
      if (kk.gt.k2) go to 80
      kk = kk + jc
      if (kk.le.mm) go to 70
      if (kk.lt.k2) go to 90
      k1 = k1 + inc + inc
      kk = (k1-kspan)/2 + jc
      if (kk.le.jc+jc) go to 60
      go to 40
  90  s1 = float((kk-1)/jc)*dr*rad
      c1 = cos(s1)
      s1 = sin(s1)
      mm = min0(k1/2,mm+klim)
      go to 80
c
c transform for factor of 3 (optional code)
c
 100  k1 = kk + kspan
      k2 = k1 + kspan
      ak = a(kk)
      bk = b(kk)
      aj = a(k1) + a(k2)
      bj = b(k1) + b(k2)
      a(kk) = ak + aj
      b(kk) = bk + bj
      ak = -0.5*aj + ak
      bk = -0.5*bj + bk
      aj = (a(k1)-a(k2))*s120
      bj = (b(k1)-b(k2))*s120
      a(k1) = ak - bj
      b(k1) = bk + aj
      a(k2) = ak + bj
      b(k2) = bk - aj
      kk = k2 + kspan
      if (kk.lt.nn) go to 100
      kk = kk - nn
      if (kk.le.kspan) go to 100
      go to 290
c
c transform for factor of 4
c
 110  if (nfac(i).ne.4) go to 230
      kspnn = kspan
      kspan = kspan/4
 120  c1 = 1.0
      s1 = 0
      mm = min0(kspan,klim)
      go to 150
 130  c2 = c1 - (cd*c1+sd*s1)
      s1 = (sd*c1-cd*s1) + s1
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, substitute
c c1=c2
c
c      c1 = 0.5/(c2**2+s1**2) + 0.5
c      s1 = c1*s1
c      c1 = c1*c2
      c1 = c2
 140  c2 = c1**2 - s1**2
      s2 = c1*s1*2.0
      c3 = c2*c1 - s2*s1
      s3 = c2*s1 + s2*c1
 150  k1 = kk + kspan
      k2 = k1 + kspan
      k3 = k2 + kspan
      akp = a(kk) + a(k2)
      akm = a(kk) - a(k2)
      ajp = a(k1) + a(k3)
      ajm = a(k1) - a(k3)
      a(kk) = akp + ajp
      ajp = akp - ajp
      bkp = b(kk) + b(k2)
      bkm = b(kk) - b(k2)
      bjp = b(k1) + b(k3)
      bjm = b(k1) - b(k3)
      b(kk) = bkp + bjp
      bjp = bkp - bjp
      if (isn.lt.0) go to 180
      akp = akm - bjm
      akm = akm + bjm
      bkp = bkm + ajm
      bkm = bkm - ajm
      if (s1.eq.0.0) go to 190
 160  a(k1) = akp*c1 - bkp*s1
      b(k1) = akp*s1 + bkp*c1
      a(k2) = ajp*c2 - bjp*s2
      b(k2) = ajp*s2 + bjp*c2
      a(k3) = akm*c3 - bkm*s3
      b(k3) = akm*s3 + bkm*c3
      kk = k3 + kspan
      if (kk.le.nt) go to 150
 170  kk = kk - nt + jc
      if (kk.le.mm) go to 130
      if (kk.lt.kspan) go to 200
      kk = kk - kspan + inc
      if (kk.le.jc) go to 120
      if (kspan.eq.jc) go to 350
      go to 40
 180  akp = akm + bjm
      akm = akm - bjm
      bkp = bkm - ajm
      bkm = bkm + ajm
      if (s1.ne.0.0) go to 160
 190  a(k1) = akp
      b(k1) = bkp
      a(k2) = ajp
      b(k2) = bjp
      a(k3) = akm
      b(k3) = bkm
      kk = k3 + kspan
      if (kk.le.nt) go to 150
      go to 170
 200  s1 = float((kk-1)/jc)*dr*rad
      c1 = cos(s1)
      s1 = sin(s1)
      mm = min0(kspan,mm+klim)
      go to 140
c
c transform for factor of 5 (optional code)
c
 210  c2 = c72**2 - s72**2
      s2 = 2.0*c72*s72
 220  k1 = kk + kspan
      k2 = k1 + kspan
      k3 = k2 + kspan
      k4 = k3 + kspan
      akp = a(k1) + a(k4)
      akm = a(k1) - a(k4)
      bkp = b(k1) + b(k4)
      bkm = b(k1) - b(k4)
      ajp = a(k2) + a(k3)
      ajm = a(k2) - a(k3)
      bjp = b(k2) + b(k3)
      bjm = b(k2) - b(k3)
      aa = a(kk)
      bb = b(kk)
      a(kk) = aa + akp + ajp
      b(kk) = bb + bkp + bjp
      ak = akp*c72 + ajp*c2 + aa
      bk = bkp*c72 + bjp*c2 + bb
      aj = akm*s72 + ajm*s2
      bj = bkm*s72 + bjm*s2
      a(k1) = ak - bj
      a(k4) = ak + bj
      b(k1) = bk + aj
      b(k4) = bk - aj
      ak = akp*c2 + ajp*c72 + aa
      bk = bkp*c2 + bjp*c72 + bb
      aj = akm*s2 - ajm*s72
      bj = bkm*s2 - bjm*s72
      a(k2) = ak - bj
      a(k3) = ak + bj
      b(k2) = bk + aj
      b(k3) = bk - aj
      kk = k4 + kspan
      if (kk.lt.nn) go to 220
      kk = kk - nn
      if (kk.le.kspan) go to 220
      go to 290
c
c transform for odd factors
c
 230  k = nfac(i)
      kspnn = kspan
      kspan = kspan/k
      if (k.eq.3) go to 100
      if (k.eq.5) go to 210
      if (k.eq.jf) go to 250
      jf = k
      s1 = rad/(float(k)/8.0)
      c1 = cos(s1)
      s1 = sin(s1)
      ck(jf) = 1.0
      sk(jf) = 0.0
      j = 1
 240  ck(j) = ck(k)*c1 + sk(k)*s1
      sk(j) = ck(k)*s1 - sk(k)*c1
      k = k - 1
      ck(k) = ck(j)
      sk(k) = -sk(j)
      j = j + 1
      if (j.lt.k) go to 240
 250  k1 = kk
      k2 = kk + kspnn
      aa = a(kk)
      bb = b(kk)
      ak = aa
      bk = bb
      j = 1
      k1 = k1 + kspan
 260  k2 = k2 - kspan
      j = j + 1
      at(j) = a(k1) + a(k2)
      ak = at(j) + ak
      bt(j) = b(k1) + b(k2)
      bk = bt(j) + bk
      j = j + 1
      at(j) = a(k1) - a(k2)
      bt(j) = b(k1) - b(k2)
      k1 = k1 + kspan
      if (k1.lt.k2) go to 260
      a(kk) = ak
      b(kk) = bk
      k1 = kk
      k2 = kk + kspnn
      j = 1
 270  k1 = k1 + kspan
      k2 = k2 - kspan
      jj = j
      ak = aa
      bk = bb
      aj = 0.0
      bj = 0.0
      k = 1
 280  k = k + 1
      ak = at(k)*ck(jj) + ak
      bk = bt(k)*ck(jj) + bk
      k = k + 1
      aj = at(k)*sk(jj) + aj
      bj = bt(k)*sk(jj) + bj
      jj = jj + j
      if (jj.gt.jf) jj = jj - jf
      if (k.lt.jf) go to 280
      k = jf - j
      a(k1) = ak - bj
      b(k1) = bk + aj
      a(k2) = ak + bj
      b(k2) = bk - aj
      j = j + 1
      if (j.lt.k) go to 270
      kk = kk + kspnn
      if (kk.le.nn) go to 250
      kk = kk - nn
      if (kk.le.kspan) go to 250
c
c multiply by rotation factor (except for factors of 2 and 4)
c
 290  if (i.eq.m) go to 350
      kk = jc + 1
 300  c2 = 1.0 - cd
      s1 = sd
      mm = min0(kspan,klim)
      go to 320
 310  c2 = c1 - (cd*c1+sd*s1)
      s1 = s1 + (sd*c1-cd*s1)
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, they may
c be deleted.
c
c      c1 = 0.5/(c2**2+s1**2) + 0.5
c      s1 = c1*s1
c      c2 = c1*c2
 320  c1 = c2
      s2 = s1
      kk = kk + kspan
 330  ak = a(kk)
      a(kk) = c2*ak - s2*b(kk)
      b(kk) = s2*ak + c2*b(kk)
      kk = kk + kspnn
      if (kk.le.nt) go to 330
      ak = s1*s2
      s2 = s1*c2 + c1*s2
      c2 = c1*c2 - ak
      kk = kk - nt + kspan
      if (kk.le.kspnn) go to 330
      kk = kk - kspnn + jc
      if (kk.le.mm) go to 310
      if (kk.lt.kspan) go to 340
      kk = kk - kspan + jc + inc
      if (kk.le.jc+jc) go to 300
      go to 40
 340  s1 = float((kk-1)/jc)*dr*rad
      c2 = cos(s1)
      s1 = sin(s1)
      mm = min0(kspan,mm+klim)
      go to 320
c
c permute the results to normal order---done in two stages
c permutation for square factors of n
c
 350  np(1) = ks
      if (kt.eq.0) go to 440
      k = kt + kt + 1
      if (m.lt.k) k = k - 1
      j = 1
      np(k+1) = jc
 360  np(j+1) = np(j)/nfac(j)
      np(k) = np(k+1)*nfac(j)
      j = j + 1
      k = k - 1
      if (j.lt.k) go to 360
      k3 = np(k+1)
      kspan = np(2)
      kk = jc + 1
      k2 = kspan + 1
      j = 1
      if (n.ne.ntot) go to 400
c
c permutation for single-variate transform (optional code)
c
 370  ak = a(kk)
      a(kk) = a(k2)
      a(k2) = ak
      bk = b(kk)
      b(kk) = b(k2)
      b(k2) = bk
      kk = kk + inc
      k2 = kspan + k2
      if (k2.lt.ks) go to 370
 380  k2 = k2 - np(j)
      j = j + 1
      k2 = np(j+1) + k2
      if (k2.gt.np(j)) go to 380
      j = 1
 390  if (kk.lt.k2) go to 370
      kk = kk + inc
      k2 = kspan + k2
      if (k2.lt.ks) go to 390
      if (kk.lt.ks) go to 380
      jc = k3
      go to 440
c
c permutation for multivariate transform
c
 400  k = kk + jc
 410  ak = a(kk)
      a(kk) = a(k2)
      a(k2) = ak
      bk = b(kk)
      b(kk) = b(k2)
      b(k2) = bk
      kk = kk + inc
      k2 = k2 + inc
      if (kk.lt.k) go to 410
      kk = kk + ks - jc
      k2 = k2 + ks - jc
      if (kk.lt.nt) go to 400
      k2 = k2 - nt + kspan
      kk = kk - nt + jc
      if (k2.lt.ks) go to 400
 420  k2 = k2 - np(j)
      j = j + 1
      k2 = np(j+1) + k2
      if (k2.gt.np(j)) go to 420
      j = 1
 430  if (kk.lt.k2) go to 400
      kk = kk + jc
      k2 = kspan + k2
      if (k2.lt.ks) go to 430
      if (kk.lt.ks) go to 420
      jc = k3
 440  if (2*kt+1.ge.m) return
      kspnn = np(kt+1)
c
c permutation for square-free factors of n
c
      j = m - kt
      nfac(j+1) = 1
 450  nfac(j) = nfac(j)*nfac(j+1)
      j = j - 1
      if (j.ne.kt) go to 450
      kt = kt + 1
      nn = nfac(kt) - 1
      jj = 0
      j = 0
      go to 480
 460  jj = jj - k2
      k2 = kk
      k = k + 1
      kk = nfac(k)
 470  jj = kk + jj
      if (jj.ge.k2) go to 460
      np(j) = jj
 480  k2 = nfac(kt)
      k = kt + 1
      kk = nfac(k)
      j = j + 1
      if (j.le.nn) go to 470
c
c determine the permutation cycles of length greater than 1
c
      j = 0
      go to 500
 490  k = kk
      kk = np(k)
      np(k) = -kk
      if (kk.ne.j) go to 490
      k3 = kk
 500  j = j + 1
      kk = np(j)
      if (kk.lt.0) go to 500
      if (kk.ne.j) go to 490
      np(j) = -j
      if (j.ne.nn) go to 500
      maxf = inc*maxf
c
c reorder a and b, following the permutation cycles
c
      go to 570
 510  j = j - 1
      if (np(j).lt.0) go to 510
      jj = jc
 520  kspan = jj
      if (jj.gt.maxf) kspan = maxf
      jj = jj - kspan
      k = np(j)
      kk = jc*k + i + jj
      k1 = kk + kspan
      k2 = 0
 530  k2 = k2 + 1
      at(k2) = a(k1)
      bt(k2) = b(k1)
      k1 = k1 - inc
      if (k1.ne.kk) go to 530
 540  k1 = kk + kspan
      k2 = k1 - jc*(k+np(k))
      k = -np(k)
 550  a(k1) = a(k2)
      b(k1) = b(k2)
      k1 = k1 - inc
      k2 = k2 - inc
      if (k1.ne.kk) go to 550
      kk = k2
      if (k.ne.j) go to 540
      k1 = kk + kspan
      k2 = 0
 560  k2 = k2 + 1
      a(k1) = at(k2)
      b(k1) = bt(k2)
      k1 = k1 - inc
      if (k1.ne.kk) go to 560
      if (jj.ne.0) go to 520
      if (j.ne.1) go to 510
 570  j = k3 + 1
      nt = nt - kspnn
      i = nt - inc + 1
      if (nt.ge.0) go to 510
      return
      end
c
c-----------------------------------------------------------------------
c subroutine:  reals
c used with 'fft' to compute fourier transform or inverse for real data
c-----------------------------------------------------------------------
c
      subroutine reals(a, b, n, isn)
c
c if isn=-1, this subroutine completes the fourier transform
c      of 2*n real data values, where the original data values are
c      stored alternately in arrays a and b, and are first
c      transformed by a complex fourier transform of dimension n.
c      the cosine coefficients are in a(1),a(2),...a(n),a(n+1)
c      and the sine coefficients are in b(1),b(2),...b(n),b(n+1).
c      note that the arrays a and b must have dimension n+1.
c      a typical calling sequence is
c        call fft(a,b,n,n,n,-1)
c        call reals(a,b,n,-1)
c
c if isn=1, the inverse transformation is done, the first
c      step in evaluating a real fourier series.
c      a typical calling sequence is
c        call reals(a,b,n,1)
c        call fft(a,b,n,n,n,1)
c      the time domain results alternate in arrays a and b,
c      i.e. a(1),b(1),a(2),b(2),...a(n),b(n).
c
c the data may alternatively be stored in a single complex
c      array a, then the magnitude of isn changed to two to
c      give the correct indexing increment and a(2) used to
c      pass the initial address for the sequence of imaginary
c      values, e.g.
c        call fft(a,a(2),n,n,n,-2)
c        call reals(a,a(2),n,-2)
c      in this case, the cosine and sine coefficients alternate in a.
c
      dimension a(1), b(1)
      inc = iabs(isn)
      nf = iabs(n)
      if (nf*isn.ne.0) go to 10
      ierr = i1mach(4)
      write (ierr,9999) n, isn
9999  format (33h error - zero in reals parameters, 2i10)
      return
c
  10  nk = nf*inc + 2
      nh = nk/2
      rad = atan(1.0)
      dr = -4.0/float(nf)
      cd = 2.0*sin(0.5*dr*rad)**2
      sd = sin(dr*rad)
c
c sin,cos values are re-initialized each lim steps
c
      lim = 32
      mm = lim
      ml = 0
      sn = 0.0
      if (isn.gt.0) go to 40
      cn = 1.0
      a(nk-1) = a(1)
      b(nk-1) = b(1)
  20  do 30 j=1,nh,inc
        k = nk - j
        aa = a(j) + a(k)
        ab = a(j) - a(k)
        ba = b(j) + b(k)
        bb = b(j) - b(k)
        re = cn*ba + sn*ab
        em = sn*ba - cn*ab
        b(k) = (em-bb)*0.5
        b(j) = (em+bb)*0.5
        a(k) = (aa-re)*0.5
        a(j) = (aa+re)*0.5
        ml = ml + 1
        if (ml.eq.mm) go to 50
        aa = cn - (cd*cn+sd*sn)
        sn = (sd*cn-cd*sn) + sn
c
c the following three statements compensate for truncation
c error.  if rounded arithmetic is used, substitute
c cn=aa
c
c        cn = 0.5/(aa**2+sn**2) + 0.5
c        sn = cn*sn
c        cn = cn*aa
      cn = aa
  30  continue
      return
c
  40  cn = -1.0
      sd = -sd
      go to 20
c
  50  mm = mm + lim
      sn = float(ml)*dr*rad
      cn = cos(sn)
      if (isn.gt.0) cn = -cn
      sn = sin(sn)
      go to 30
      end
c
c-----------------------------------------------------------------------
c function:  istkgt(nitems,itype)
c allocates working storage for nitems of itype, as follows
c
c 1 - logical
c 2 - integer
c 3 - real
c 4 - double precision
c 5 - complex
c
c-----------------------------------------------------------------------
c
      integer function istkgt(nitems, itype)
c
      common /cstak/ dstak(2500)
c
      double precision dstak
      integer istak(5000)
      integer isize(5)
c
      equivalence (dstak(1),istak(1))
      equivalence (istak(1),lout)
      equivalence (istak(2),lnow)
      equivalence (istak(3),lused)
      equivalence (istak(4),lmax)
      equivalence (istak(5),lbook)
      equivalence (istak(6),isize(1))
c
      istkgt = (lnow*isize(2)-1)/isize(itype) + 2
      i = ((istkgt-1+nitems)*isize(itype)-1)/isize(2) + 3
      if (i.gt.lmax) go to 10
      istak(i-1) = itype
      istak(i) = lnow
      lout = lout + 1
      lnow = i
      lused = max0(lused,lnow)
      return
c
  10  ierr = i1mach(4)
      write (ierr,9999) i
9999  format (1h , 39hoverflow of common array istak --- need, i10)
      write (ierr,9998) (istak(j),j=1,10), istak(lnow-1), istak(lnow)
9998  format (12i6)
      stop
      end
c
c-----------------------------------------------------------------------
c subroutine:  istkrl(k)
c de-allocates the last k working storage areas
c-----------------------------------------------------------------------
c
      subroutine istkrl(k)
c
      common /cstak/ dstak(2500)
c
      double precision dstak
      integer istak(5000)
c
      equivalence (dstak(1),istak(1))
      equivalence (istak(1),lout)
      equivalence (istak(2),lnow)
      equivalence (istak(3),lused)
      equivalence (istak(4),lmax)
      equivalence (istak(5),lbook)
c
      in = k
c
      if (lbook.le.lnow .and. lnow.le.lused .and. lused.le.lmax) go to
     *    10
      ierr = i1mach(4)
      write (ierr,9999)
9999  format (53h warning...istak(2),istak(3),istak(4) or istak(5) hit)
      write (ierr,9997) (istak(j),j=1,10), istak(lnow-1), istak(lnow)
c
  10  if (in.le.0) return
      if (lbook.gt.istak(lnow) .or. istak(lnow).ge.lnow-1) go to 20
      lout = lout - 1
      lnow = istak(lnow)
      in = in - 1
      go to 10
c
  20  ierr = i1mach(4)
      write (ierr,9998)
9998  format (45h warning...pointer at istak(lnow) overwritten/11x,
     *    27hde-allocation not completed)
      write (ierr,9997) (istak(j),j=1,10), istak(lnow-1), istak(lnow)
9997  format (12i6)
      return
c
      end
