
c-----------------------------------------------------------------------
c subroutine:  covar
c a subroutine for implementing the covariance
c method of linear prediction analysis
c-----------------------------------------------------------------------
c
      subroutine covar(n, x, m, a, alpha, grc)
c
c   inputs:   n - no. of data points
c             x(n) - input data sequence
c             m - order of filter (m<21, see note*)
c  outputs:   a - filter coefficients
c             alpha - residual "energy"
c             a - filter coefficients
c             grc - "generalized reflection coefficients",
c
c *program limited to m=20, because of the dimensions
c b(m*(m+1)/2), beta(m), and cc(m+1)
c
      dimension x(1), a(1), grc(1)
      dimension b(190), beta(20), cc(21)
      mp = m + 1
      mt = mp*m/2
      mt = (mp*m)/2
      do 10 j=1,mt
        b(j) = 0.
  10  continue
      alpha = 0.
      cc(1) = 0.
      cc(2) = 0.
      do 20 np=mp,n
        np1 = np - 1
        alpha = alpha + x(np)*x(np)
        cc(1) = cc(1) + x(np)*x(np1)
        cc(2) = cc(2) + x(np1)*x(np1)
  20  continue
      b(1) = 1.
      beta(1) = cc(2)
      grc(1) = -cc(1)/cc(2)
      a(1) = 1.
      a(2) = grc(1)
      alpha = alpha + grc(1)*cc(1)
      mf = m
      do 130 minc=2,mf
        do 30 j=1,minc
          jp = minc + 2 - j
          n1 = mp + 1 - jp
          n2 = n + 1 - minc
          n3 = n + 2 - jp
          n4 = mp - minc
          cc(jp) = cc(jp-1) + x(n4)*x(n1) - x(n2)*x(n3)
  30    continue
        cc(1) = 0.
        do 40 np=mp,n
          n1 = np - minc
          cc(1) = cc(1) + x(n1)*x(np)
  40    continue
        msub = (minc*minc-minc)/2
        mm1 = minc - 1
        n1 = msub + minc
        b(n1) = 1.
        do 80 ip=1,mm1
          isub = (ip*ip-ip)/2
          if (beta(ip)) 150, 150, 50
  50      gam = 0.
          do 60 j=1,ip
            n1 = isub + j
            gam = gam + cc(j+1)*b(n1)
  60      continue
          gam = gam/beta(ip)
          do 70 jp=1,ip
            n1 = msub + jp
            n2 = isub + jp
            b(n1) = b(n1) - gam*b(n2)
  70      continue
  80    continue
        beta(minc) = 0.
        do 90 j=1,minc
          n1 = msub + j
          beta(minc) = beta(minc) + cc(j+1)*b(n1)
  90    continue
        if (beta(minc)) 150, 150, 100
 100    s = 0.
        do 110 ip=1,minc
          s = s + cc(ip)*a(ip)
 110    continue
        grc(minc) = -s/beta(minc)
        do 120 ip=2,minc
          m2 = msub + ip - 1
          a(ip) = a(ip) + grc(minc)*b(m2)
 120    continue
        a(minc+1) = grc(minc)
        s = grc(minc)*grc(minc)*beta(minc)
        alpha = alpha - s
        if (alpha) 150, 150, 130
 130  continue
 140  return
 150  continue
c
c warning - singular matrix
c
      ioutd = i1mach(2)
      write (ioutd,9999)
9999  format (34h warning - singular matrix - covar)
      go to 140
      end
c
c-----------------------------------------------------------------------
c subroutine:  auto
c a subroutine for implementing the autocorrelation
c method of linear prediction analysis
c-----------------------------------------------------------------------
c
      subroutine auto(n, x, m, a, alpha, rc)
c
c   inputs:   n - no. of data points
c             x(n) - input data sequence
c             m - order of filter (m<21, see note*)
c  outputs:   a - filter coefficients
c             alpha - residual "energy"
c             rc - reflection coefficients
c
c *program limited to m<21 because of dimensions of r(.)
c
      dimension x(1), a(1), rc(1)
      dimension r(21)
      mp = m + 1
      do 20 k=1,mp
        r(k) = 0.
        nk = n - k + 1
        do 10 np=1,nk
          n1 = np + k - 1
          r(k) = r(k) + x(np)*x(n1)
  10    continue
  20  continue
      rc(1) = -r(2)/r(1)
      a(1) = 1.
      a(2) = rc(1)
      alpha = r(1) + r(2)*rc(1)
      do 50 minc=2,m
        s = 0.
        do 30 ip=1,minc
          n1 = minc - ip + 2
          s = s + r(n1)*a(ip)
  30    continue
        rc(minc) = -s/alpha
        mh = minc/2 + 1
        do 40 ip=2,mh
          ib = minc - ip + 2
          at = a(ip) + rc(minc)*a(ib)
          a(ib) = a(ib) + rc(minc)*a(ip)
          a(ip) = at
  40    continue
        a(minc+1) = rc(minc)
        alpha = alpha + rc(minc)*s
        if (alpha) 70, 70, 50
  50  continue
  60  return
  70  continue
c
c warning - singular matrix
c
      ioutd = i1mach(2)
      write (ioutd,9999)
9999  format (33h warning - singular matrix - auto)
      go to 60
      end
c
