c
c-----------------------------------------------------------------------
c main program: test program for decimation,interpolation and filtering
c author:       r e crochiere, l r rabiner
c               bell laboratories, murray hill, new jersey, 07974
c input:        coef1 = array of filter coefficients for first stage
c                       for test problem 1.
c               coef2 = array of filter coefficients for second stage
c                       for test problem 1.
c               coef11 = array of filter coefficients for first stage
c                        for 1-stage example in test problem 3.
c               coef1a = array of filter coefficients for first stage
c                        for 3-stage example in test problem 3.
c               coef2a = array of filter coefficients for second stage
c                        for 3-stage example in test problem 3.
c               coef3a = array of filter coefficients for third stage
c                        for 3-stage example in test problem 3.
c-----------------------------------------------------------------------
c
      common /difc1/ k, id1, id2, id3, n1, n2, n3, itype, id, isb, n1p,
     *    n2p, n3p
      common /difc2/ idd, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11,
     *    j12, j1s, j2s, j3s, ncf1, ncf2, ncf3, nf1, nf2, nf3, iq1,
     *    iq2, iq3, k1s, k2s, k3s
      dimension x(60), coef1(13), coef2(14), sbuff(106), buff(20),
     *    coef3(1)
      dimension coef11(60)
      dimension coef1a(25), coef2a(20), coef3a(75)
      integer tti, tto
c
c define i/o device codes
c input: input to this program is user interactive
c        that is - a question is written on the user
c        terminal (tto) and the user types in the answer.
c
c output: all output is written on the standard
c         output unit (lpt).
c
      tti = i1mach(1)
      tto = i1mach(4)
      lpt = i1mach(2)
c
c test example 1
c
      a = 0.9
      x(1) = -a
      x(2) = 1. - a*a
      do 10 i=3,60
        x(i) = x(i-1)*a
  10  continue
      read (tti,9999) (coef1(i),i=1,13)
      read (tti,9999) (coef2(i),i=1,14)
      write (tto,9998)
      write (tto,9999) (coef1(i),i=1,13)
      write (tto,9997)
      write (tto,9999) (coef2(i),i=1,14)
9999  format (7f10.6)
9998  format (25h coefficients for stage 1)
9997  format (25h coefficients for stage 2)
      call diinit(2, 5, 2, 1, 25, 28, 0, coef1, coef2, coef3, 1, buff,
     *    20, sbuff, 106, ier)
      if (ier.ne.0) stop
      do 30 i=1,3
        do 20 j=1,20
          jx = (i-1)*20 + j
          buff(j) = x(jx)
  20    continue
        write (tto,9996)
9996    format (15h input sequence)
        write (tto,9995) (buff(j),j=1,20)
9995    format (1h , 10f7.4)
        call difilt(coef1, coef2, coef3, buff, sbuff)
        write (tto,9994)
9994    format (26h decimated output sequence)
        write (tto,9995) (buff(j),j=1,2)
  30  continue
c
c test example 2
c restore coef1(1)
c
      coef1(1) = coef1(1)*2.
      call diinit(2, 5, 2, 1, 25, 28, 0, coef1, coef2, coef3, 2, buff,
     *    20, sbuff, 106, ier)
      if (ier.ne.0) stop
      write (tto,9993)
9993  format (1h /1h )
      do 50 i=1,3
        do 40 j=1,2
          jx = (i-1)*2 + j
          buff(j) = x(jx)
  40    continue
        write (tto,9996)
        write (tto,9995) (buff(j),j=1,2)
        call difilt(coef1, coef2, coef3, buff, sbuff)
        write (tto,9992)
9992    format (29h interpolated output sequence)
        write (tto,9995) (buff(j),j=1,20)
  50  continue
c
c test example 3
c
      write (tto,9993)
      read (tti,9999) (coef11(i),i=1,60)
      write (tto,9991)
9991  format (34h coefficients for a 1-stage design)
      write (tto,9999) (coef11(i),i=1,60)
      write (tto,9993)
      read (tti,9999) (coef1a(i),i=1,25)
      read (tti,9999) (coef2a(i),i=1,20)
      read (tti,9999) (coef3a(i),i=1,75)
      write (tto,9990)
9990  format (34h coefficients for a 3-stage design)
      write (tto,9989)
      write (tto,9999) (coef1a(i),i=1,25)
      write (tto,9988)
      write (tto,9999) (coef2a(i),i=1,20)
      write (tto,9987)
      write (tto,9999) (coef3a(i),i=1,75)
9989  format (8h stage 1)
9988  format (8h stage 2)
9987  format (8h stage 3)
      stop
      end
c
c-----------------------------------------------------------------------
c subroutine: diinit
c initialization for difilt which decimates, interpolates or
c filters a signal
c-----------------------------------------------------------------------
c
      subroutine diinit(kd, id1d, id2d, id3d, n1d, n2d, n3d, coef1,
     *    coef2, coef3, ityped, buff, idjd, sbuff, isbd, ierr)
      common /difc1/ k, id1, id2, id3, n1, n2, n3, itype, id, isb, n1p,
     *    n2p, n3p
      common /difc2/ idd, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11,
     *    j12, j1s, j2s, j3s, ncf1, ncf2, ncf3, nf1, nf2, nf3, iq1,
     *    iq2, iq3, k1s, k2s, k3s
      dimension coef1(1), coef2(1), coef3(1), sbuff(1), buff(1)
c
c     kd = no. of stages of decimation and /or interpolation
c   id1d = decimation ratio for stage 1
c   id2d = decimation ratio for stage 2 = 1 if kd=1
c   id3d = decimation ratio for stage 3 = 1 if kd=1 or 2
c    n1d = filter length for stage 1
c    n2d = filter length for stage 2 = 0 if kd = 1
c    n3d = filter length for stage 3 = 0 if kd = 1 or 2
c  coef1 = coef. array for stage 1     (size = [(n1d+1)/2] )[x]=integer
c  coef2 = coef. array for stage 2     (size = [(n2d+1)/2] )    part of
c  coef3 = coef. array for stage 3     (size = [(n3d+1)/2] )
c ityped = 1 for decimator
c        = 2 for interpolator
c        = 3 for filter
c   buff = storage buffer for input/output
c   idjd = size of buff = integer*id1d*id2d*id3d
c  sbuff = scratch storage buffer for internal variables
c   isbd = size of sbuff
c        = 2*(n1p+n2p+n3p)                for decimator
c        = (n1p+n2p+n3p)+id1d+id2d        for interpolator
c        = 3*(n1p+n2p+n3p)+id1d+id2d      for filter
c where  n1p = ([n1d/id1d]+1)*id1d       [x]=integer part of x
c        n2p = ([n2d/id2d]+1)*id2d           or x-1 if x=integer
c        n3p = ([n3d/id3d]+1)*id3d
c   ierr = error code for debugging purposes
c        = 0    no errors encountered in initialization
c        = 1    kd not equal to 1, 2, or 3
c        = 2    idjd not an integer multiple of id1d*id2d*id3d
c
c initialization and setup
c transfer of dummy arguments to common
c
      k = kd
      id1 = id1d
      id2 = id2d
      id3 = id3d
      n1 = n1d
      n2 = n2d
      n3 = n3d
      itype = ityped
      id = idjd
      isb = isbd
      ierr = 0
c
c preliminary checks
c
      if ((k.gt.3) .or. (k.lt.1)) ierr = 1
      m = 1
      if (k.eq.3) go to 10
      n3 = 0
      id3 = 0
      iq3 = 0
      if (k.eq.2) go to 20
      n2 = 0
      id2 = 0
      iq2 = 0
      go to 30
  10  m = m*id3
      iq3 = n3/id3
      if (n3.ne.(iq3*id3)) iq3 = iq3 + 1
  20  m = m*id2
      iq2 = n2/id2
      if (n2.ne.(iq2*id2)) iq2 = iq2 + 1
  30  m = m*id1
      iq1 = n1/id1
      if (n1.ne.(iq1*id1)) iq1 = iq1 + 1
      idd = id/m
      if (id.ne.(m*idd)) ierr = 2
      n1p = iq1*id1
      n2p = iq2*id2
      n3p = iq3*id3
c
c setup of address locations in sbuff for internal storage buffers
c and zero out sbuff for initialization
c
      j1 = 0
      j2 = j1 + 2*n1p
      j3 = j2 + 2*n2p
      j4 = j3 + 2*n3p
      if (itype.eq.2) j4 = 0
      j5 = j4 + iq1*id1
      j6 = j5 + iq2*id2
      j7 = j6 + iq3*id3
      j8 = j7 + 2*iq1
      j9 = j8 + 2*iq2
      j10 = j9 + 2*iq3
      j11 = j10 + id2
      j12 = j11 + id3
      if (itype.eq.1) j12 = j4
      if (isb.lt.j12) ierr = 3
      do 40 m=1,j12
        sbuff(m) = 0.0
  40  continue
c
c setup of scrambled coefficient sets for interpolation,
c initialization of moving address pointers and
c halving of first coefficient in sets coef1 to coef3
c
c stage 1
c
      idx = j4 + 1
      ncf1 = (n1+1)/2
      sum = id1
      if (itype.eq.1) go to 70
      do 60 md=1,id1
        do 50 mq=1,iq1
          m = (md-1) + (mq-1)*id1
          if (m.lt.ncf1) mm = ncf1 - m
          if (m.ge.ncf1) mm = m - (n1-ncf1-1)
          if (mm.le.ncf1) sbuff(idx) = coef1(mm)*sum
          if (mm.gt.ncf1) sbuff(idx) = 0.
          idx = idx + 1
  50    continue
  60  continue
  70  if (n1.eq.(2*ncf1-1)) coef1(1) = coef1(1)/2.0
      nf1 = (n1+2)/2
      j1s = j1 + n1p
      k1s = j7 + iq1
      if (k.lt.2) go to 140
c
c stage 2
c
      idx = j5 + 1
      ncf2 = (n2+1)/2
      sum = id2
      if (itype.eq.1) go to 100
      do 90 md=1,id2
        do 80 mq=1,iq2
          m = (md-1) + (mq-1)*id2
          if (m.lt.ncf2) mm = ncf2 - m
          if (m.ge.ncf2) mm = m - (n2-ncf2-1)
          if (mm.le.ncf2) sbuff(idx) = coef2(mm)*sum
          if (mm.gt.ncf2) sbuff(idx) = 0.
          idx = idx + 1
  80    continue
  90  continue
 100  if (n2.eq.(2*ncf2-1)) coef2(1) = coef2(1)/2.0
      nf2 = (n2+2)/2
      j2s = j2 + n2p
      k2s = j8 + iq2
      if (k.lt.3) go to 140
c
c stage 3
c
      idx = j6 + 1
      ncf3 = (n3+1)/2
      sum = id3
      if (itype.eq.1) go to 130
      do 120 md=1,id3
        do 110 mq=1,iq3
          m = (md-1) + (mq-1)*id3
          if (m.lt.ncf3) mm = ncf3 - m
          if (m.ge.ncf3) mm = m - (n3-ncf3-1)
          if (mm.le.ncf3) sbuff(idx) = coef3(mm)*sum
          if (mm.gt.ncf3) sbuff(idx) = 0.
          idx = idx + 1
 110    continue
 120  continue
 130  if (n3.eq.(2*ncf3-1)) coef3(1) = coef3(1)/2.0
      nf3 = (n3+2)/2
      j3s = j3 + n3p
      k3s = j9 + iq3
 140  return
      end
c
c-----------------------------------------------------------------------
c subroutine: difilt
c difilt decimates, interpolates or filters a signal,
c diinit must be called prior to calling this routine
c-----------------------------------------------------------------------
c
c for decimator, fill buff with idjd samples, call difilt, and
c     receive (idjd/id1d*id2d*id3d) samples
c for interpolator, supply (idjd/id1d*id2d*id3d) input samples
c     to buff, call difilt, and receive idjd output samples
c for filter, supply and receive idjd samples from buff
c
      subroutine difilt(coef1, coef2, coef3, buff, sbuff)
      common /difc1/ k, id1, id2, id3, n1, n2, n3, itype, id, isb, n1p,
     *    n2p, n3p
      common /difc2/ idd, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11,
     *    j12, j1s, j2s, j3s, ncf1, ncf2, ncf3, nf1, nf2, nf3, iq1,
     *    iq2, iq3, k1s, k2s, k3s
      dimension coef1(1), coef2(1), coef3(1), sbuff(1), buff(1)
c
c  coef1 = coef. array for stage 1     (size = [(n1d+1)/2] )[x]=integer
c  coef2 = coef. array for stage 2     (size = [(n2d+1)/2] )    part of
c  coef3 = coef. array for stage 3     (size = [(n3d+1)/2] )
c   buff = storage buffer for input/output
c  sbuff = scratch storage buffer for internal variables
c
      mm = 1
      nn = 1
      if (itype.ne.2) go to 20
      mm = id - idd + 1
      do 10 m=1,idd
        buff(mm) = buff(m)
        mm = mm + 1
  10  continue
      mm = id - idd + 1
  20  if (k.eq.2) go to 100
      if (k.eq.3) go to 220
c
c one stage decimation and/or interpolation
c this section of code is removable if 1 stage (kd=1) implementation
c are not used        (substitute       200 go to 2000)
c one stage decimation
c read id1 samples from buff into s1d buffer
c
c
  30  if (itype.ne.2) go to 40
      sum = buff(mm)
      mm = mm + 1
      go to 70
  40  m1 = j1s - id1
      md = j1s + n1p
  50  sbuff(j1s) = buff(mm)
      sbuff(md) = buff(mm)
      mm = mm + 1
      j1s = j1s - 1
      md = md - 1
      if (m1.lt.j1s) go to 50
c
c compute one filter output for stage 1d
c
      jd = j1s + ncf1
      ju = j1s + nf1
      sum = 0.0
      m1 = 0
  60  m1 = m1 + 1
      sum = sum + coef1(m1)*(sbuff(ju)+sbuff(jd))
      jd = jd - 1
      ju = ju + 1
      if (m1.lt.ncf1) go to 60
      if (j1s.le.j1) j1s = j1 + n1p
      if (itype.ne.1) go to 70
      buff(nn) = sum
      nn = nn + 1
      if (mm.le.id) go to 40
      go to 390
c
c one stage interpolation
c store data into s1i
c
  70  sbuff(k1s) = sum
      md = k1s + iq1
      sbuff(md) = sum
      mq = j4
c
c compute id1 samples from stage 1i
c
  80  m1 = k1s
      sum = 0.0
  90  mq = mq + 1
      sum = sum + sbuff(mq)*sbuff(m1)
      m1 = m1 + 1
      if (m1.lt.md) go to 90
c
c store output into buff
c
      buff(nn) = sum
      nn = nn + 1
      if (mq.lt.j5) go to 80
      k1s = k1s - 1
      if (k1s.le.j7) k1s = j7 + iq1
      if (mm.le.id) go to 30
      go to 390
c
c two stage decimation and/or interpolation:
c this section of code is removable if 2 stage (kd=2) implementation
c are not used        (substitute       300 go to 2000)
c two stage decimation
c
 100  if (itype.ne.2) go to 110
      sum = buff(mm)
      mm = mm + 1
      go to 160
 110  m2 = j2s - id2
      mq = j2s + n2p
c
c read id1 samples from buff into s1d buffer
c
 120  m1 = j1s - id1
      md = j1s + n1p
 130  sbuff(j1s) = buff(mm)
      sbuff(md) = buff(mm)
      mm = mm + 1
      j1s = j1s - 1
      md = md - 1
      if (m1.lt.j1s) go to 130
c
c compute one filter output for stage 1d
c
      jd = j1s + ncf1
      ju = j1s + nf1
      sum = 0.0
      m1 = 0
 140  m1 = m1 + 1
      sum = sum + coef1(m1)*(sbuff(jd)+sbuff(ju))
      jd = jd - 1
      ju = ju + 1
      if (m1.lt.ncf1) go to 140
      if (j1s.le.j1) j1s = j1 + n1p
c
c store data into s2d
c
      sbuff(j2s) = sum
      sbuff(mq) = sum
      j2s = j2s - 1
      mq = mq - 1
      if (m2.lt.j2s) go to 120
c
c compute one filter output for stage 2d
c
      jd = j2s + ncf2
      ju = j2s + nf2
      sum = 0.0
      m2 = 0
 150  m2 = m2 + 1
      sum = sum + coef2(m2)*(sbuff(jd)+sbuff(ju))
      jd = jd - 1
      ju = ju + 1
      if (m2.lt.ncf2) go to 150
      if (j2s.le.j2) j2s = j2 + n2p
      if (itype.ne.1) go to 160
      buff(nn) = sum
      nn = nn + 1
      if (mm.le.id) go to 110
      go to 390
c
c two stage interpolation
c store data into s2i
c
 160  sbuff(k2s) = sum
      md = k2s + iq2
      sbuff(md) = sum
      idx = j10 + 1
      mq = j5
c
c compute output for stage 2i
c
 170  m2 = k2s
      sum = 0.0
 180  mq = mq + 1
      sum = sum + sbuff(mq)*sbuff(m2)
      m2 = m2 + 1
      if (m2.lt.md) go to 180
c
c store output in t2i
c
      sbuff(idx) = sum
      idx = idx + 1
      if (idx.le.j11) go to 170
      k2s = k2s - 1
      if (k2s.le.j8) k2s = j8 + iq2
      idx = j10 + 1
c
c store data into s1i
c
 190  sbuff(k1s) = sbuff(idx)
      md = k1s + iq1
      sbuff(md) = sbuff(idx)
      idx = idx + 1
      mq = j4
 200  m1 = k1s
      sum = 0.0
 210  mq = mq + 1
      sum = sum + sbuff(mq)*sbuff(m1)
      m1 = m1 + 1
      if (m1.lt.md) go to 210
      buff(nn) = sum
      nn = nn + 1
      if (mq.lt.j5) go to 200
      k1s = k1s - 1
      if (k1s.le.j7) k1s = j7 + iq1
      if (idx.le.j11) go to 190
      if (mm.le.id) go to 100
      go to 390
c
c three stage decimation and/or interpolation
c this section of code is removable if 3 stage (kd=3) implementation
c three stage decimation
c
 220  if (itype.ne.2) go to 230
      sum = buff(mm)
      mm = mm + 1
      go to 300
 230  m3 = j3s - id3
      m = j3s + n3p
 240  m2 = j2s - id2
      mq = j2s + n2p
c
c read id1 samples from buff into s1d buffer
c
 250  m1 = j1s - id1
      md = j1s + n1p
 260  sbuff(j1s) = buff(mm)
      sbuff(md) = buff(mm)
      mm = mm + 1
      j1s = j1s - 1
      md = md - 1
      if (m1.lt.j1s) go to 260
c
c compute one filter output for stage 1d
c
      jd = j1s + ncf1
      ju = j1s + nf1
      sum = 0.0
      m1 = 0
 270  m1 = m1 + 1
      sum = sum + coef1(m1)*(sbuff(jd)+sbuff(ju))
      jd = jd - 1
      ju = ju + 1
      if (m1.lt.ncf1) go to 270
      if (j1s.le.j1) j1s = j1 + n1p
c
c store data into s2d
c
      sbuff(j2s) = sum
      sbuff(mq) = sum
      j2s = j2s - 1
      mq = mq - 1
      if (m2.lt.j2s) go to 250
c
c compute one filter output for stage 2d
c
      jd = j2s + ncf2
      ju = j2s + nf2
      sum = 0.0
      m2 = 0
 280  m2 = m2 + 1
      sum = sum + coef2(m2)*(sbuff(jd)+sbuff(ju))
      jd = jd - 1
      ju = ju + 1
      if (m2.lt.ncf2) go to 280
      if (j2s.le.j2) j2s = j2 + n2p
c
c store data into s3d
c
      sbuff(j3s) = sum
      sbuff(m) = sum
      j3s = j3s - 1
      m = m - 1
      if (m3.lt.j3s) go to 240
c
c compute one filter output for stage 3d
c
      jd = j3s + ncf3
      ju = j3s + nf3
      sum = 0.0
      m3 = 0
 290  m3 = m3 + 1
      sum = sum + coef3(m3)*(sbuff(jd)+sbuff(ju))
      jd = jd - 1
      ju = ju + 1
      if (m3.lt.ncf3) go to 290
      if (j3s.le.j3) j3s = j3 + n3p
      if (itype.ne.1) go to 300
      buff(nn) = sum
      nn = nn + 1
      if (mm.le.id) go to 230
      go to 390
c
c three stage interpolation
c store data into s3i
c
 300  sbuff(k3s) = sum
      md = k3s + iq3
      sbuff(md) = sum
      m = j11 + 1
      mq = j6
c
c compute output for stage 3i
c
 310  m3 = k3s
      sum = 0.0
 320  mq = mq + 1
      sum = sum + sbuff(mq)*sbuff(m3)
      m3 = m3 + 1
      if (m3.lt.md) go to 320
c
c store output in t3i
c
      sbuff(m) = sum
      m = m + 1
      if (m.le.j12) go to 310
      k3s = k3s - 1
      if (k3s.le.j9) k3s = j9 + iq3
      m = j11 + 1
c
c store data into s2i
c
 330  sbuff(k2s) = sbuff(m)
      md = k2s + iq2
      sbuff(md) = sbuff(m)
      m = m + 1
      idx = j10 + 1
      mq = j5
c
c compute output for stage 2i
c
 340  m2 = k2s
      sum = 0.0
 350  mq = mq + 1
      sum = sum + sbuff(mq)*sbuff(m2)
      m2 = m2 + 1
      if (m2.lt.md) go to 350
c
c store output in t2i
c
      sbuff(idx) = sum
      idx = idx + 1
      if (idx.le.j11) go to 340
      k2s = k2s - 1
      if (k2s.le.j8) k2s = j8 + iq2
      idx = j10 + 1
c
c store data into s1i
c
 360  sbuff(k1s) = sbuff(idx)
      md = k1s + iq1
      sbuff(md) = sbuff(idx)
      idx = idx + 1
      mq = j4
c
c compute output for stage 1i
c
 370  m1 = k1s
      sum = 0.0
 380  mq = mq + 1
      sum = sum + sbuff(mq)*sbuff(m1)
      m1 = m1 + 1
      if (m1.lt.md) go to 380
c
c store output in buff
c
      buff(nn) = sum
      nn = nn + 1
      if (mq.lt.j5) go to 370
      k1s = k1s - 1
      if (k1s.le.j7) k1s = j7 + iq1
      if (idx.le.j11) go to 360
      if (m.le.j12) go to 330
      if (mm.le.id) go to 220
 390  return
      end
