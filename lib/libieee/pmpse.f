c
c-----------------------------------------------------------------------
c main program: modified periodogram method for power spectrum
c               estimation-pmpse
c authors:      l r rabiner
c               bell laboratories, murray hill, new jersey 07974
c               r w schafer and d dlugos
c               dept of electrical engineering
c               georgia institute of technology
c               atlanta, georgia 30332
c
c method based on technique described by p d welch, ieee
c trans on audio and elect, vol 15, no 2, pp 70-73, 1967.
c
c input:        m is the fft length (must be a power of 2)
c                   2 <= m <= maxm (1024)
c               iwin is the window type
c                   iwin = 1  rectangular window
c                   iwin = 2  hamming window
c               l is the window length
c                   l <= m
c               n is the maximum number of samples available
c                   for analysis
c               fs is the sampling frequency in hz
c               imd requests additional runs
c                   imd = 1   new run
c                   imd = 0   terminates program
c-----------------------------------------------------------------------
c
      dimension xa(1024), xfr(513), sxx(513), wd(1024)
      character jwin(2,4)
      dimension ilag(513)
      complex x(1024), xmn
      data jwin(1,1), jwin(1,2), jwin(1,3), jwin(1,4) /"r","e","c","t"/
      data jwin(2,1), jwin(2,2), jwin(2,3), jwin(2,4) /"h","a","m","g"/
c
c define i/o device codes
c input: input to this is user-interactive
c        that is - a question is written on the user
c        terminal (iout1) and the user types in the answer.
c
c output: all output is written on the standard
c         output unit (iout2).
c
      ind = i1mach(1)
      iout1 = i1mach(4)
      iout2 = i1mach(2)
c
c set maximum fft size maxm depending on dimension within program
c
      maxm = 1024
      lhm = maxm/2 + 1
c
c fill lag array for printing
c
      do 10 i=1,lhm
        ilag(i) = i - 1
  10  continue
  20  continue
c
c read in analysis parameters m,iwin,l,n,fs
c
      write (iout1,9999)
9999  format (16h fft length(i4)=)
      read (ind,9997) m
      if (m.gt.maxm) write (iout1,9998)
9998  format (27h m too large--reenter value)
      if (m.gt.maxm) go to 20
9997  format (i4)
      write (iout1,9996)
9996  format (43h window type(i1)   1=rectangular, 2=hamming)
      read (ind,9995) iwin
9995  format (i1)
      write (iout1,9994)
9994  format (19h window length(i4)=)
      read (ind,9997) l
      write (iout1,9993)
9993  format (40h maximum number of analysis samples(i5)=)
      read (ind,9992) n
9992  format (i5)
      write (iout1,9991)
9991  format (33h sampling frequency in hz(f10.4)=)
      read (ind,9990) fs
9990  format (f10.4)
c
c nsect = the total number of analysis sections
c    np = the total number of samples actually used
c overlap of 2 to 1 is used on adjacent analysis sections
c    np = n if (n-l/2)/(l/2) = an integer
c
      mhlf1 = m/2 + 1
      nsect = (n-l/2)/(l/2)
      np = nsect*(l/2) + l/2
      write (iout2,9989) jwin(iwin,1), jwin(iwin,2), jwin(iwin,3),
     *    jwin(iwin,4)
9989  format (13h window type=, 4a1)
      write (iout2,9987)
      write (iout2,9988) m, np, l, fs
9988  format (3h m=, i4, 5h  np=, i5, 4h  l=, i4, 18h  sampling frequen,
     *    3hcy=, f10.4)
c
c calculate mean of data.
c
      ss = 1.
      xsum = 0.
      ns1 = nsect + 1
      l1 = l/2
      do 40 k=1,ns1
        call getx(xa, l, ss)
        do 30 i=1,l1
          xsum = xsum + xa(i)
  30    continue
        ss = ss + float(l1)
  40  continue
      xmean = xsum/float(np)
      xmn = cmplx(xmean,xmean)
      write (iout2,9987)
9987  format (//)
      write (iout2,9986) xmean
9986  format (7h xmean=, e14.5)
c
c generate window
c
      u = float(l)
      if (iwin.ne.2) go to 60
      u = 0.
      fl = float(l-1)
      twopi = 8.*atan(1.0)
      do 50 i=1,l
        fi = float(i-1)
        wd(i) = .54 - .46*cos(twopi*fi/fl)
        u = u + wd(i)*wd(i)
  50  continue
  60  continue
c
c loop to accumulate spectra 2 at a time
c
      ss = 1.
      do 70 i=1,mhlf1
        sxx(i) = 0.
  70  continue
c
c read l/2 samples to initialize buffer
c
      nrd = l/2
      l2 = l/2 + 1
      call getx(xa(l2), nrd, ss)
      ss = ss + float(nrd)
      imn = l/2 + 1
      kmx = (nsect+1)/2
      nsectp = (nsect+1)/2
      nrd = l
      do 190 k=1,kmx
c
c move down upper half of xa buffer
c
        do 80 i=1,l1
          j = l1 + i
          x(i) = cmplx(xa(j),0.)
  80    continue
        if (k.ne.kmx .or. nsectp.eq.nsect) go to 100
        do 90 i=imn,nrd
          xa(i) = 0.0
  90    continue
        nrd = l/2
 100    call getx(xa, nrd, ss)
        do 110 i=1,l1
          j = i + l1
          x(j) = cmplx(xa(i),xa(j)) - xmn
          x(i) = cmplx(real(x(i)),xa(i)) - xmn
 110    continue
        if (k.ne.kmx .or. nsectp.eq.nsect) go to 130
c
c an odd number of sections--zero out the second part
c
        do 120 i=1,l
          x(i) = cmplx(real(x(i)),0.)
 120    continue
 130    continue
        ss = ss + float(nrd)
        if (iwin.ne.2) go to 150
        do 140 i=1,l
          x(i) = x(i)*wd(i)
 140    continue
 150    continue
        if (l.eq.m) go to 170
        lp1 = l + 1
        do 160 i=lp1,m
          x(i) = (0.,0.)
 160    continue
 170    continue
        call fftps(x, m, 0)
        do 180 i=2,mhlf1
          j = m + 2 - i
          sxx(i) = sxx(i) + real(x(i)*conjg(x(i))+x(j)*conjg(x(j)))
 180    continue
        sxx(1) = sxx(1) + real(x(1)*conjg(x(1)))*2.
 190  continue
c
c normalize spectral estimate and obtain correlation function
c using inverse fft of power spectrum
c
      fnorm = 2.*u*float(nsect)
      do 200 i=1,mhlf1
        sxx(i) = sxx(i)/fnorm
        x(i) = cmplx(sxx(i),0.)
        j = m + 2 - i
        x(j) = x(i)
 200  continue
      call fftps(x, m, 1)
      do 210 i=1,mhlf1
        xa(i) = real(x(i))
 210  continue
c
c correlation estimate is in xa from 1 to mhlf1
c compute log of power spectrum estimate
c
      xfs = fs/float(m)
      do 220 i=1,mhlf1
        xfr(i) = float(i-1)*xfs
        tmp = alog10(sxx(i))
        sxx(i) = 20.*tmp
 220  continue
c
c log power spectrum (db) is in array sxx
c if desired, the user may insert code at this point to plot the log
c power spectrum
c
      write (iout2,9987)
      write (iout2,9985)
9985  format (19h log power spectrum)
      write (iout2,9987)
      write (iout2,9984)
9984  format (5x, 4hfreq, 7x, 2hdb, 5x, 4hfreq, 7x, 2hdb, 5x, 4hfreq,
     *    7x, 2hdb, 5x, 4hfreq, 7x, 2hdb)
      write (iout2,9983) (xfr(i),sxx(i),i=1,mhlf1)
9983  format (4(f9.3, f9.4))
c
c correlation function is in array xa
c if desired, the user may insert code at this point to plot the
c correlation function
c
      write (iout2,9987)
      write (iout2,9982)
9982  format (21h correlation function)
      write (iout2,9987)
      write (iout2,9981)
9981  format (1x, 3hlag, 2x, 4hcorr, 5x, 3hlag, 2x, 4hcorr, 5x, 3hlag,
     *    2x, 4hcorr, 5x, 3hlag, 2x, 4hcorr, 5x, 3hlag, 2x, 4hcorr)
      write (iout2,9980) (ilag(i),xa(i),i=1,mhlf1)
9980  format (5(i4, e10.3))
      write (iout2,9987)
      write (iout2,9979)
9979  format (////)
      write (iout1,9978)
9978  format (23h more data(1=yes,0=no)=)
      read (ind,9995) imd
      if (imd.eq.1) go to 20
      stop
      end
c
c-----------------------------------------------------------------------
c subroutine:  getx
c read data routine generate x(n) for a sine input of frequency fs/10.
c where fs is the sampling frequency in hz
c-----------------------------------------------------------------------
c
      subroutine getx(x, nrd, ss)
      dimension x(1)
c
c   x = array of size nrd to hold generator output data
c nrd = number of samples to be created
c  ss = starting sample of generator output
c sine wave frequency is 1000 hz with an assumed sampling frequency
c of 10000 hz
c
      tpi = 8.*atan(1.0)
      cf = 1000./10000.
      do 10 i=1,nrd
        xsmp = (ss-1.) + float(i-1)
        x(i) = cos(tpi*cf*xsmp)
  10  continue
      return
      end
c
c-----------------------------------------------------------------------
c subroutine:  fftps
c jim cooley's simple fft program--uses decimation in time algorithm
c x is an n=2**m point complex array that initially contains the input
c and on output contains the transform
c the parameter inv specified direct transform if 0 and inverse if 1
c-----------------------------------------------------------------------
c
      subroutine fftps(x, n, inv)
      complex x(1), u, w, t, cmplx
c
c   x = complex array of size n--on input x contains
c       the sequence to be transformed
c       on output x contains the dft of the input
c   n = size of fft to be computed--n=2**m for 1.le.m.le.15
c inv = parameter to determine whether to do a direct transform (inv=0)
c       or an inverse transform (inv=1)
c
      m = alog(float(n))/alog(2.) + .1
      nv2 = n/2
      nm1 = n - 1
      j = 1
      do 40 i=1,nm1
        if (i.ge.j) go to 10
        t = x(j)
        x(j) = x(i)
        x(i) = t
  10    k = nv2
  20    if (k.ge.j) go to 30
        j = j - k
        k = k/2
        go to 20
  30    j = j + k
  40  continue
      pi = 4.*atan(1.0)
      do 70 l=1,m
        le = 2**l
        le1 = le/2
        u = (1.0,0.0)
        w = cmplx(cos(pi/float(le1)),-sin(pi/float(le1)))
        if (inv.ne.0) w = conjg(w)
        do 60 j=1,le1
          do 50 i=j,n,le
            ip = i + le1
            t = x(ip)*u
            x(ip) = x(i) - t
            x(i) = x(i) + t
  50      continue
          u = u*w
  60    continue
  70  continue
      if (inv.eq.0) return
      do 80 i=1,n
        x(i) = x(i)/cmplx(float(n),0.)
  80  continue
      return
      end
