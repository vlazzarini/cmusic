c
c-----------------------------------------------------------------------
c  main program: test program for optimal digital interpolating filter
c  authors:      geerd oetken, h. w. schussler and t. w. parks
c                t. w. parks
c                dept. of electrical engineering
c                rice univ.
c                houston, texas   77001
c
c                g. oetken and h. w. schussler
c                institut fuer nachrichtentechnik
c                universitaet erlangen-nuernberg
c                8520 erlan
c                cauerstrasse 7
c                west germany
c
c  input:        num is the number of times to loop calling dodif
c                l determines the degree of the filter
c                    the degree is 2*l*r
c                alpha is the cutoff frequency (radians) of the filter
c                    input data.  range: 0.d0 < alpha <= 1.d0
c                r is the wanted increase in the sampling rate
c                    range: an integer > 1
c-----------------------------------------------------------------------
c
c     double precision alpha, dif(100)
c     integer r
c
c define i/o device codes
c
c     in = i1mach(1)
c     iout = i1mach(2)
c
c     read (in,9999) num
c 9999  format (i5)
c     do 20 ii=1,num
c       read (in,9998) l, alpha, r
c 9998    format (i5, f5.3, i5)
c       lr = l*r + 1
c       write (iout,9997)
c 9997    format (1h0, 35h  optimal interpolating filter data)
c       write (iout,9996)
c 9996    format (1h0)
c       write (iout,9995) l, r, alpha
c 9995    format (1h0, 5h  l= , i2, 10h       r= , i2, 14h       alpha= ,
c    *      f4.2)
c       write (iout,9996)
c       call dodif(l, r, alpha, dif)
c       write (iout,9994) lr
c 9994    format (1h0, 10h there are, i5, 28h values in half of the filte,
c    *      1hr)
c       write (iout,9996)
c       j = -lr
c       do 10 i=1,lr
c         j = j + 1
c         write (iout,9993) j, dif(i)
c 10    continue
c 9993    format (1h0, 4h  h(, i3, 2h)=, e16.7)
c 20  continue
c     stop
c     end
c
c-----------------------------------------------------------------------
c subroutine: dodif
c subroutine to calculate the optimal digital interpolating
c filter
c-----------------------------------------------------------------------
c
      subroutine dodif(l, r, alpha, dif)
      double precision ap(100), am(100), d(200)
      double precision dble, dfloat, s1, s2, dphi, alpha, h, x, y,
     *    dif(1)
      integer r
c
c input:
c      r     -wanted increase in sampling rate, i.e. the filter
c                  calculates r-1 samples between each pair of
c                  input samples. range for r/ any integer > 1
c      l     -integer which determines the degree of the filter.
c                  the degree is 2*l*r.
c                  range for l/ 1 to 10, may be increased at the
c                  risk of numerical instability (change lmax and
c                  array bounds of ap,am,d for l > 10).
c      alpha -cutoff frequency (radians) of the filter input data
c                  divided by pi/r.
c                  range for alpha/ 0.d0< alpha <= 1.d0 (double
c                  precision).
c      dif   -double precision array in which the first half (l*r+1
c                  values) of the unit sample response is returned.
c                  (the optimal filter has linear phase).
c
c fortran 4 usage/
c      call dodif(l,r,alpha,dif)
c
c remarks/
c      big values of l and small values of alpha increase the
c      possibility of problems with matrix inversion (depending upon
c      the wordlength of the computer). a very good possibility to
c      control the quality of the matrix inversion is to check the
c      magnitude of the unit sample response dif(k) at k=m*r+1,
c      m=0 to l-1, which ideally have to be zero.
c
c subroutines and functions required/
c      dphi, sminvd
c
      data lmax /10/
c
c (maximum value of l specified by array bounds)
c
      data eps /1.e-6/
c
c (relative tolerance for loss of significance test in matrix)
c
      dfloat(if) = dble(float(if))
      iodev = i1mach(2)
c
c (device no. on which error messages are to happen)
c
c
c test if l within current bounds
c
      if (l.gt.lmax) go to 100
c
c calculate the two l*l matrices
c
      lr = 0
      do 20 m=1,l
        do 10 k=1,m
          k1 = k + lr
          s1 = k - m
          s1 = dabs(s1)
          s2 = dfloat(2*l+1-m-k)
          s1 = dphi(s1,alpha)
          s2 = dphi(s2,alpha)
          ap(k1) = s1 + s2
          am(k1) = s1 - s2
  10    continue
        lr = lr + m
  20  continue
c
c invert both matrices using ibm/ssp subroutines
c
      call sminvd(ap, l, eps, ier1)
      call sminvd(am, l, eps, ier2)
      if (ier1.eq.(-1) .or. ier2.eq.(-1)) go to 110
c
c calculate the l*2l matrix d
c
      do 40 m=1,l
        m1 = (m-1)*l
        do 30 k=1,l
          k1 = 2*(m1+k) - 1
          k2 = (k-1)*k/2 + m
          if (m.gt.k) k2 = (m-1)*m/2 + k
          s1 = ap(k2) + am(k2)
          s2 = ap(k2) - am(k2)
          d(k1) = s1
          d(k1+1) = s2
  30    continue
  40  continue
c
c calculate unit sample response from d and dphi
c
      j = 0
      do 90 i=1,l
        i1 = (i-1)*2*l
        do 80 m=1,r
          m1 = m - 1
          go to 60
  50      m1 = r
  60      x = dfloat(m1)/dfloat(r)
          h = 0.d0
          do 70 k=1,l
            y = dfloat(l-(k-1)) - x
            it = i1 + 2*k - 1
            h = h + d(it)*dphi(y,alpha)
            y = dfloat(l-k) + x
            it = i1 + 2*k
            h = h + d(it)*dphi(y,alpha)
  70      continue
          j = j + 1
c
c first half of unit sample response is stored in dif(j)
c
          dif(j) = .5d0*h
          if (i.eq.l .and. m1.eq.(r-1)) go to 50
  80    continue
  90  continue
      return
c
c error messages
c
 100  write (iodev,9999) lmax
9999  format (18h l is greater than, i4)
      go to 120
 110  write (iodev,9998)
9998  format (54h error during matrix inversion, ill conditioned system)
 120  return
      end
c
c-----------------------------------------------------------------------
c function: dphi
c function required by dodif
c special version for lowpass interpolators
c-----------------------------------------------------------------------
c
      double precision function dphi(x, alpha)
      double precision x, alpha, pi
      pi = 4.d0*datan(1.d0)
      if (dabs(x).lt.1.d-10) go to 10
      x = alpha*pi*x
      dphi = dsin(x)/x
      return
  10  continue
      dphi = 1.d0
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: sminvd
c author: dean kolba
c         dept. of elec. eng.
c         rice university
c         houston, tx. 77001
c
c subroutine to invert a positive-definite symmetric matrix
c-----------------------------------------------------------------------
c
      subroutine sminvd(a, n, eps, ier)
      double precision a(1), sum, ad
c
c calculate the cholesky decomposition
c
      ier = 0
      id = 0
      do 70 k=1,n
        id = id + k
        iv = id
        km1 = k - 1
        test = eps*sngl(a(id))
        do 60 i=k,n
          sum = 0.d0
          if (km1.eq.0) go to 20
          do 10 j=1,km1
            j1 = id - j
            j2 = iv - j
            sum = sum + a(j1)*a(j2)
  10      continue
  20      sum = a(iv) - sum
          if (i.gt.k) go to 40
          if (sngl(sum).gt.abs(test)) go to 30
          if (sum.le.0.d0) go to 150
          if (ier.gt.0) go to 30
          ier = km1
  30      a(id) = dsqrt(sum)
          go to 50
  40      a(iv) = sum/a(id)
  50      iv = iv + i
  60    continue
  70  continue
c
c invert upper triangular matrix resulting from cholesky decomposition
c
      nv = n*(n+1)/2
      id = nv
      do 110 k=1,n
        a(id) = 1.d0/a(id)
        jh = n
        km1 = k - 1
        if (km1.eq.0) go to 100
        jl = n - km1
        iv = nv - k + 1
        do 90 i=1,km1
          sum = 0.d0
          jh = jh - 1
          j1 = id
          j2 = iv
          do 80 j=jl,jh
            j1 = j1 + j
            j2 = j2 + 1
            sum = sum + a(j1)*a(j2)
  80      continue
          a(iv) = -sum*a(id)
          iv = iv - jh
  90    continue
 100    id = id - jh
 110  continue
c
c calculate the inverse of a using the inverted triangular matrix
c
      id = 0
      do 140 k=1,n
        id = id + k
        iv = id
        do 130 i=k,n
          sum = 0.d0
          j1 = iv
          jl = i - k
          do 120 j=i,n
            j2 = j1 + jl
            sum = sum + a(j1)*a(j2)
            j1 = j1 + j
 120      continue
          a(iv) = sum
          iv = iv + i
 130    continue
 140  continue
      return
 150  ier = -1
      return
c
c ier=-1   inversion failed
c ier=0    good inversion
c ier=i    loss of significance at the i+1 stage
c
      end
