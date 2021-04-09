c
c revised 20 APR 81: frm
c-----------------------------------------------------------------------
c subroutine:  lptrn
c this subroutine carries out the transformations between
c various parameter sets used in linear prediction
c-----------------------------------------------------------------------
c
      subroutine lptrn(i, m, a, c, r, rc, alar, area, alpha)
c
c  inputs:  i  -  variable identifying which are inputs
c           m  -  filter order (m<51, see note below*)
c
c     i=1     input=a(.) & alpha
c     i=2     input=c(.)
c     i=3     input=r(.)
c     i=4     input=rc(.) & r(1)
c     i=5     input=alar(.) & r(1)
c     i=6     input=area(.) & r(1)
c
c        a(.)    =  filter coef.
c        alpha   =  gain
c        c(.)    =  cepstral coef.
c        r(.)    =  autocorrelation coef.
c        r(1)    =  first auto. coef. (energy)
c        rc(.)   =  reflection coef.
c        alar(.) =  log area ratios
c        area(.) =  area functions
c
c    * program limited to m=50 by dimension sa(50),
c      a temporary storage for filter coefficients
c
      dimension a(1), c(1), r(1), rc(1), alar(1), area(1)
      dimension sa(50)
      common ioutd
c
c test for m out of range
c
      mtest = (m-1)*(50-m)
      if (mtest) 340, 340, 10
  10  mp = m + 1
      if (i-2) 50, 20, 50
c
c ..generates a(.) ,alpha, from c(.)
c
  20  alpha = exp(c(1))
      a(1) = 1.
      do 40 k=1,m
        kp = k + 1
        sum = 0.
        do 30 j=1,k
          jb = k - j + 2
          sum = sum + a(j)*c(jb)*float(jb-1)
  30    continue
        a(kp) = -sum/float(k)
  40  continue
  50  go to (110, 110, 160, 60, 200, 220), i
c
c ..generates sa(.),r(.), & alpha from rc(.) & r(1)
c
  60  do 70 j=1,m
        sa(j) = rc(j)
  70  continue
      r(2) = -rc(1)*r(1)
      alpha = r(1)*(1.-rc(1)*rc(1))
      do 100 j=2,m
        mh = j/2
        q = rc(j)
        alpha = alpha*(1.-q*q)
        do 80 k=1,mh
          kb = j - k
          at = sa(k) + q*sa(kb)
          sa(kb) = sa(kb) + q*sa(k)
          sa(k) = at
  80    continue
        sum = 0.
        do 90 l=1,j
          lb = j + 1 - l
          sum = sum + sa(l)*r(lb)
  90    continue
        r(j+1) = -sum
 100  continue
      if (i-4) 240, 240, 260
 110  do 120 j=1,m
        sa(j) = a(j+1)
 120  continue
c
c ..generates rc(.),r(1), from a(.) & alpha
c
      do 130 j=1,m
        rc(j) = sa(j)
 130  continue
      alt = alpha
      do 150 j=2,m
c 
        jb = m + 2 - j
        mh = jb/2
        rct = rc(jb)
        dr = 1./(1. - rct*rct)
c
        do 140 k=1,mh
c
          kb = jb - k
          q = (rc(k)-rct*rc(kb))*dr
	  rc(kb) = (rc(kb) - rct*rc(k))*dr
          rc(k) = q
 140    continue
        alt = alt*dr
c
 150  continue
      r(1) = alt/(1.-rc(1)*rc(1))
      go to 60
c
c ..generates rc(.),sa(.),& alpha from r(.)
c
 160  rc(1) = -r(2)/r(1)
      sa(1) = rc(1)
      alpha = r(1)*(1.-rc(1)*rc(1))
      do 190 j=2,m
        mh = j/2
        jm = j - 1
        q = r(j+1)
        do 170 l=1,jm
          lb = j + 1 - l
          q = q + sa(l)*r(lb)
 170    continue
        q = -q/alpha
        rc(j) = q
        do 180 k=1,mh
          kb = j - k
c
          at = sa(k) + q*sa(kb)
	  sa(kb) = sa(kb) + q*sa(k)
c
          sa(k) = at
 180    continue
        sa(j) = q
        alpha = alpha*(1.-q*q)
 190  continue
      go to 240
c
c ..generates rc(.) & area(.) from alar(.)
c
 200  area(mp) = 1.
      do 210 j=1,m
        jb = m + 1 - j
        ar = exp(alar(jb))
        rc(jb) = (1.-ar)/(1.+ar)
        area(jb) = area(jb+1)/ar
 210  continue
      go to 60
c
c ..generates alrar(.) & rc(.) from area(.)
c
 220  do 230 j=1,m
        ar = area(j+1)/area(j)
        alar(j) = alog(ar)
        rc(j) = (1.-ar)/(1.+ar)
 230  continue
      go to 60
c
c ..generates area(.) & alar(.) from rc(.)
c
 240  area(mp) = 1.
      do 250 j=1,m
        jb = m + 1 - j
        ar = (1.-rc(jb))/(1.+rc(jb))
        alar(jb) = alog(ar)
        area(jb) = area(jb+1)/ar
 250  continue
      if (i-2) 280, 310, 260
 260  do 270 j=2,mp
        a(j) = sa(j-1)
 270  continue
      a(1) = 1.
c
c ..generate c(.) from a(.) & alpha
c
 280  c(1) = alog(alpha)
      c(2) = -a(2)
      do 300 l=2,m
        lp = l + 1
        sum = float(l)*a(lp)
        do 290 j=2,l
          jb = l - j + 2
          sum = sum + a(j)*c(jb)*float(jb-1)
 290    continue
        c(lp) = -sum/float(l)
 300  continue
 310  do 330 j=1,m
        if (abs(rc(j))-1.) 330, 320, 320
 320    write (ioutd,9999)
 330  continue
9999  format (19h filter is unstable)
      return
 340  write (ioutd,9998) m
9998  format (3h m=, i6, 13h out of range)
      return
      end
