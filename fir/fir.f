c-----------------------------------------------------------------------
c main program: fir linear phase filter design program
c
c authors: james h. mcclellan
c          department of electrical engineering and computer science
c          massachusetts institute of technology
c          cambridge, mass. 02139
c
c          thomas w. parks
c          department of electrical engineering
c          rice university
c          houston, texas 77001
c
c          lawrence r. rabiner
c          bell laboratories
c          murray hill, new jersey 07974
c
c input:
c  nfilt-- filter length
c  jtype-- type of filter
c          1 = multiple passband/stopband filter
c          2 = differentiator
c          3 = hilbert transform filter
c  nbands-- number of bands
c  lgrid-- grid density, will be set to 16 unless
c          specified otherwise by a positive constant.
c
c  edge(2*nbands)-- bandedge array, lower and upper edges for each band
c                   with a maximum of 10 bands.
c
c  fx(nbands)-- desired function array (or desired slope if a
c               differentiator) for each band.
c
c  wtx(nbands)-- weight function array in each band.  for a
c                differentiator, the weight function is inversely
c                proportional to f.
c
c  sample input data setup:
c       32,1,3,0
c       0.0,0.1,0.2,0.35,0.425,0.5
c       0.0,1.0,0.0
c       10.0,1.0,10.0
c     this data specifies a length 32 bandpass filter with
c     stopbands 0 to 0.1 and 0.425 to 0.5, and passband from
c     0.2 to 0.35 with weighting of 10 in the stopbands and 1
c     in the passband.  the grid density defaults to 16.
c     this is the filter in figure 10.
c
c     the following input data specifies a length 32 fullband
c     differentiator with slope 1 and weighting of 1/f.
c     the grid density will be set to 20.
c       32,2,1,20
c       0,0.5
c       1.0
c       1.0
c
c-----------------------------------------------------------------------
c
c     The '1h1' carriage control formats in the program output section 
c     have been changed to '1h ' --- jos : 13-aug-80
c
c     The load module firsub.o contains the routines 'input' and 'putfir'
c     which are the only departure from the original portable form
c     of this program. --- jos : 12-aug-80
c
      common pi2,ad,dev,x,y,grid,des,wt,alpha,iext,nfcns,ngrid
      common /oops/niter,iout
      dimension iext(66),ad(66),alpha(66),x(66),y(66)
      dimension h(66)
      dimension des(1045),grid(1045),wt(1045)
      dimension edge(20),fx(10),wtx(10),deviat(10)
      double precision pi2,pi
      double precision ad,dev,x,y
      double precision gee,d
      character bd1,bd2,bd3,bd4
      data bd1,bd2,bd3,bd4/"b","a","n","d"/
      iin=i1mach(1)
      iout=i1mach(2)
      pi=4.0*datan(1.0d0)
      pi2=2.0d00*pi
c
c  the program is set up for a maximum length of 128, but
c  this upper limit can be changed by redimensioning the
c  arrays iext, ad, alpha, x, y, h to be nfmax/2 + 2.
c  the arrays des, grid, and wt must dimensioned
c  16(nfmax/2 + 2).
c
      nfmax=128
  100 continue
      jtype=0
      nbands=0
      lgrid=0
c
c     max order is 128, max number of bands is 10:
      nbmax=10
      call input(nfilt,nbands,jtype,lgrid,jb,edge,fx,wtx,nfmax,nbmax)
c
      if(nfilt.le.nfmax.and.nfilt.ge.3) go to 125
      if(jtype.gt.0.and.jtype.le.3) go to 125
      call error
      stop
  125 neg=1
      if(jtype.eq.1) neg=0
      nodd=nfilt/2
      nodd=nfilt-2*nodd
      nfcns=nfilt/2
      if(nodd.eq.1.and.neg.eq.0) nfcns=nfcns+1
c
c  set up the dense grid.  the number of points in the grid
c  is (filter length + 1)*grid density/2
c
      grid(1)=edge(1)
      delf=lgrid*nfcns
      delf=0.5/delf
      if(neg.eq.0) go to 135
      if(edge(1).lt.delf) grid(1)=delf
  135 continue
      j=1
      l=1
      lband=1
  140 fup=edge(l+1)
  145 temp=grid(j)
c
c  calculate the desired magnitude response and the weight
c  function on the grid
c
      des(j)=eff(temp,fx,wtx,lband,jtype)
      wt(j)=wate(temp,fx,wtx,lband,jtype)
      j=j+1
      grid(j)=temp+delf
      if(grid(j).gt.fup) go to 150
      go to 145
  150 grid(j-1)=fup
      des(j-1)=eff(fup,fx,wtx,lband,jtype)
      wt(j-1)=wate(fup,fx,wtx,lband,jtype)
      lband=lband+1
      l=l+2
      if(lband.gt.nbands) go to 160
      grid(j)=edge(l)
      go to 140
  160 ngrid=j-1
      if(neg.ne.nodd) go to 165
      if(grid(ngrid).gt.(0.5-delf)) ngrid=ngrid-1
  165 continue
c
c  set up a new approximation problem which is equivalent
c  to the original problem
c
      if(neg) 170,170,180
  170 if(nodd.eq.1) go to 200
      do 175 j=1,ngrid
      change=dcos(pi*grid(j))
      des(j)=des(j)/change
  175 wt(j)=wt(j)*change
      go to 200
  180 if(nodd.eq.1) go to 190
      do 185 j=1,ngrid
      change=dsin(pi*grid(j))
      des(j)=des(j)/change
  185 wt(j)=wt(j)*change
      go to 200
  190 do 195 j=1,ngrid
      change=dsin(pi2*grid(j))
      des(j)=des(j)/change
  195 wt(j)=wt(j)*change
c
c  initial guess for the extremal frequencies--equally
c  spaced along the grid
c
  200 temp=float(ngrid-1)/float(nfcns)
      do 210 j=1,nfcns
      xt=j-1
  210 iext(j)=xt*temp+1.0
      iext(nfcns+1)=ngrid
      nm1=nfcns-1
      nz=nfcns+1
c
c  call the remez exchange algorithm to do the approximation
c  problem
c
      call remez
c
c  calculate the impulse response.
c
      if(neg) 300,300,320
  300 if(nodd.eq.0) go to 310
      do 305 j=1,nm1
      nzmj=nz-j
  305 h(j)=0.5*alpha(nzmj)
      h(nfcns)=alpha(1)
      go to 350
  310 h(1)=0.25*alpha(nfcns)
      do 315 j=2,nm1
      nzmj=nz-j
      nf2j=nfcns+2-j
  315 h(j)=0.25*(alpha(nzmj)+alpha(nf2j))
      h(nfcns)=0.5*alpha(1)+0.25*alpha(2)
      go to 350
  320 if(nodd.eq.0) go to 330
      h(1)=0.25*alpha(nfcns)
      h(2)=0.25*alpha(nm1)
      do 325 j=3,nm1
      nzmj=nz-j
      nf3j=nfcns+3-j
  325 h(j)=0.25*(alpha(nzmj)-alpha(nf3j))
      h(nfcns)=0.5*alpha(1)-0.25*alpha(3)
      h(nz)=0.0
      go to 350
  330 h(1)=0.25*alpha(nfcns)
      do 335 j=2,nm1
      nzmj=nz-j
      nf2j=nfcns+2-j
  335 h(j)=0.25*(alpha(nzmj)-alpha(nf2j))
      h(nfcns)=0.5*alpha(1)-0.25*alpha(2)
c
c  program output section.
c
  350 write(iout,360)
  360 format(1h , 70(1h*)//15x,29hfinite impulse response (fir)/
     113x,34hlinear phase digital filter design/
     217x,24hremez exchange algorithm/)
      if(jtype.eq.1) write(iout,365)
  365 format(22x,15hbandpass filter/)
      if(jtype.eq.2) write(iout,370)
  370 format(22x,14hdifferentiator/)
      if(jtype.eq.3) write(iout,375)
  375 format(20x,19hhilbert transformer/)
      write(iout,378) nfilt
  378 format(20x,16hfilter length = ,i3/)
      write(iout,380)
  380 format(15x,28h***** impulse response *****)
      do 381 j=1,nfcns
      k=nfilt+1-j
      if(neg.eq.0) write(iout,382) j,h(j),k
      if(neg.eq.1) write(iout,383) j,h(j),k
  381 continue
  382 format(13x,2hh(,i2,4h) = ,e15.8,5h = h(,i3,1h))
  383 format(13x,2hh(,i2,4h) = ,e15.8,6h = -h(,i3,1h))
      if(neg.eq.1.and.nodd.eq.1) write(iout,384) nz
  384 format(13x,2hh(,i2,8h) =  0.0)
      do 450 k=1,nbands,4
      kup=k+3
      if(kup.gt.nbands) kup=nbands
      write(iout,385) (bd1,bd2,bd3,bd4,j,j=k,kup)
  385 format(/24x,4(4a1,i3,7x))
      write(iout,390) (edge(2*j-1),j=k,kup)
  390 format(2x,15hlower band edge,5f14.7)
      write(iout,395) (edge(2*j),j=k,kup)
  395 format(2x,15hupper band edge,5f14.7)
      if(jtype.ne.2) write(iout,400) (fx(j),j=k,kup)
  400 format(2x,13hdesired value,2x,5f14.7)
      if(jtype.eq.2) write(iout,405) (fx(j),j=k,kup)
  405 format(2x,13hdesired slope,2x,5f14.7)
      write(iout,410) (wtx(j),j=k,kup)
  410 format(2x,9hweighting,6x,5f14.7)
      do 420 j=k,kup
  420 deviat(j)=dev/wtx(j)
      write(iout,425) (deviat(j),j=k,kup)
  425 format(2x,9hdeviation,6x,5f14.7)
      if(jtype.ne.1) go to 450
      do 430 j=k,kup
  430 deviat(j)=20.0*alog10(deviat(j)+fx(j))
      write(iout,435) (deviat(j),j=k,kup)
  435 format(2x,15hdeviation in db,5f14.7)
  450 continue
      do 452 j=1,nz
      ix=iext(j)
  452 grid(j)=grid(ix)
      write(iout,455) (grid(j),j=1,nz)
  455 format(/2x,47hextremal frequencies--maxima of the error curve/
     1 (2x,5f12.7))
      write(iout,460)
  460 format(/1x,70(1h*)/1h )
c
c Unix io:
c
	call putfir(nfilt,neg,h,jtype,lgrid,nbands,edge,fx,wtx,deviat)
c     
      go to 100
      end
c-----------------------------------------------------------------------
c function: eff
c   function to calculate the desired magnitude response
c   as a function of frequency.
c   an arbitrary function of frequency can be
c   approximated if the user replaces this function
c   with the appropriate code to evaluate the ideal
c   magnitude.  note that the parameter freq is the
c   value of normalized frequency needed for evaluation.
c-----------------------------------------------------------------------
c
      function eff(freq,fx,wtx,lband,jtype)
      dimension fx(5),wtx(5)
      if(jtype.eq.2) go to 1
      eff=fx(lband)
      return
    1 eff=fx(lband)*freq
      return
      end
c
c-----------------------------------------------------------------------
c function: wate
c   function to calculate the weight function as a function
c   of frequency.  similar to the function eff, this function can
c   be replaced by a user-written routine to calculate any
c   desired weighting function.
c-----------------------------------------------------------------------
c
      function wate(freq,fx,wtx,lband,jtype)
      dimension fx(5),wtx(5)
      if(jtype.eq.2) go to 1
      wate=wtx(lband)
      return
    1 if(fx(lband).lt.0.0001) go to 2
      wate=wtx(lband)/freq
      return
    2 wate=wtx(lband)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: error
c   this routine writes an error message if an
c   error has been detected in the input data.
c-----------------------------------------------------------------------
c
      subroutine error
      common /oops/niter,iout
      write(iout,1)
    1 format(44h ************ error in input data **********)
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: remez
c   this subroutine implements the remez exchange algorithm
c   for the weighted chebyshev approximation of a continuous
c   function with a sum of cosines.  inputs to the subroutine
c   are a dense grid which replaces the frequency axis, the
c   desired function on this grid, the weight function on the
c   grid, the number of cosines, and an initial guess of the
c   extremal frequencies.  the program minimizes the chebyshev
c   error by determining the best location of the extremal
c   frequencies (points of maximum error) and then calculates
c   the coefficients of the best approximation.
c-----------------------------------------------------------------------
c
      subroutine remez
      common pi2,ad,dev,x,y,grid,des,wt,alpha,iext,nfcns,ngrid
      common /oops/niter,iout
      dimension iext(66),ad(66),alpha(66),x(66),y(66)
      dimension des(1045),grid(1045),wt(1045)
      dimension a(66),p(65),q(65)
      double precision pi2,dnum,dden,dtemp,a,p,q
      double precision dk,dak
      double precision ad,dev,x,y
      double precision gee,d
c
c  the program allows a maximum number of iterations of 25
c
      itrmax=25
      devl=-1.0
      nz=nfcns+1
      nzz=nfcns+2
      niter=0
  100 continue
      iext(nzz)=ngrid+1
      niter=niter+1
      if(niter.gt.itrmax) go to 400
      do 110 j=1,nz
      jxt=iext(j)
      dtemp=grid(jxt)
      dtemp=dcos(dtemp*pi2)
  110 x(j)=dtemp
      jet=(nfcns-1)/15+1
      do 120 j=1,nz
  120 ad(j)=d(j,nz,jet)
      dnum=0.0
      dden=0.0
      k=1
      do 130 j=1,nz
      l=iext(j)
      dtemp=ad(j)*des(l)
      dnum=dnum+dtemp
      dtemp=float(k)*ad(j)/wt(l)
      dden=dden+dtemp
  130 k=-k
      dev=dnum/dden
      write(iout,131) dev
  131 format(1x,12hdeviation = ,f12.9)
      nu=1
      if(dev.gt.0.0) nu=-1
      dev=-float(nu)*dev
      k=nu
      do 140 j=1,nz
      l=iext(j)
      dtemp=float(k)*dev/wt(l)
      y(j)=des(l)+dtemp
  140 k=-k
      if(dev.gt.devl) go to 150
      call ouch
      go to 400
  150 devl=dev
      jchnge=0
      k1=iext(1)
      knz=iext(nz)
      klow=0
      nut=-nu
      j=1
c
c  search for the extremal frequencies of the best
c  approximation
c
  200 if(j.eq.nzz) ynz=comp
      if(j.ge.nzz) go to 300
      kup=iext(j+1)
      l=iext(j)+1
      nut=-nut
      if(j.eq.2) y1=comp
      comp=dev
      if(l.ge.kup) go to 220
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.le.0.0) go to 220
      comp=float(nut)*err
  210 l=l+1
      if(l.ge.kup) go to 215
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.le.0.0) go to 215
      comp=float(nut)*err
      go to 210
  215 iext(j)=l-1
      j=j+1
      klow=l-1
      jchnge=jchnge+1
      go to 200
  220 l=l-1
  225 l=l-1
      if(l.le.klow) go to 250
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.gt.0.0) go to 230
      if(jchnge.le.0) go to 225
      go to 260
  230 comp=float(nut)*err
  235 l=l-1
      if(l.le.klow) go to 240
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.le.0.0) go to 240
      comp=float(nut)*err
      go to 235
  240 klow=iext(j)
      iext(j)=l+1
      j=j+1
      jchnge=jchnge+1
      go to 200
  250 l=iext(j)+1
      if(jchnge.gt.0) go to 215
  255 l=l+1
      if(l.ge.kup) go to 260
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.le.0.0) go to 255
      comp=float(nut)*err
      go to 210
  260 klow=iext(j)
      j=j+1
      go to 200
  300 if(j.gt.nzz) go to 320
      if(k1.gt.iext(1)) k1=iext(1)
      if(knz.lt.iext(nz)) knz=iext(nz)
      nut1=nut
      nut=-nu
      l=0
      kup=k1
      comp=ynz*(1.00001)
      luck=1
  310 l=l+1
      if(l.ge.kup) go to 315
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.le.0.0) go to 310
      comp=float(nut)*err
      j=nzz
      go to 210
  315 luck=6
      go to 325
  320 if(luck.gt.9) go to 350
      if(comp.gt.y1) y1=comp
      k1=iext(nzz)
  325 l=ngrid+1
      klow=knz
      nut=-nut1
      comp=y1*(1.00001)
  330 l=l-1
      if(l.le.klow) go to 340
      err=gee(l,nz)
      err=(err-des(l))*wt(l)
      dtemp=float(nut)*err-comp
      if(dtemp.le.0.0) go to 330
      j=nzz
      comp=float(nut)*err
      luck=luck+10
      go to 235
  340 if(luck.eq.6) go to 370
      do 345 j=1,nfcns
      nzzmj=nzz-j
      nzmj=nz-j
  345 iext(nzzmj)=iext(nzmj)
      iext(1)=k1
      go to 100
  350 kn=iext(nzz)
      do 360 j=1,nfcns
  360 iext(j)=iext(j+1)
      iext(nz)=kn
      go to 100
  370 if(jchnge.gt.0) go to 100
c
c  calculation of the coefficients of the best approximation
c  using the inverse discrete fourier transform
c
  400 continue
      nm1=nfcns-1
      fsh=1.0e-06
      gtemp=grid(1)
      x(nzz)=-2.0
      cn=2*nfcns-1
      delf=1.0/cn
      l=1
      kkk=0
      if(grid(1).lt.0.01.and.grid(ngrid).gt.0.49) kkk=1
      if(nfcns.le.3) kkk=1
      if(kkk.eq.1) go to 405
      dtemp=dcos(pi2*grid(1))
      dnum=dcos(pi2*grid(ngrid))
      aa=2.0/(dtemp-dnum)
      bb=-(dtemp+dnum)/(dtemp-dnum)
  405 continue
      do 430 j=1,nfcns
      ft=j-1
      ft=ft*delf
      xt=dcos(pi2*ft)
      if(kkk.eq.1) go to 410
      xt=(xt-bb)/aa
      xt1=sqrt(1.0-xt*xt)
      ft=atan2(xt1,xt)/pi2
  410 xe=x(l)
      if(xt.gt.xe) go to 420
      if((xe-xt).lt.fsh) go to 415
      l=l+1
      go to 410
  415 a(j)=y(l)
      go to 425
  420 if((xt-xe).lt.fsh) go to 415
      grid(1)=ft
      a(j)=gee(1,nz)
  425 continue
      if(l.gt.1) l=l-1
  430 continue
      grid(1)=gtemp
      dden=pi2/cn
      do 510 j=1,nfcns
      dtemp=0.0
      dnum=j-1
      dnum=dnum*dden
      if(nm1.lt.1) go to 505
      do 500 k=1,nm1
      dak=a(k+1)
      dk=k
  500 dtemp=dtemp+dak*dcos(dnum*dk)
  505 dtemp=2.0*dtemp+a(1)
  510 alpha(j)=dtemp
      do 550 j=2,nfcns
  550 alpha(j)=2.0*alpha(j)/cn
      alpha(1)=alpha(1)/cn
      if(kkk.eq.1) go to 545
      p(1)=2.0*alpha(nfcns)*bb+alpha(nm1)
      p(2)=2.0*aa*alpha(nfcns)
      q(1)=alpha(nfcns-2)-alpha(nfcns)
      do 540 j=2,nm1
      if(j.lt.nm1) go to 515
      aa=0.5*aa
      bb=0.5*bb
  515 continue
      p(j+1)=0.0
      do 520 k=1,j
      a(k)=p(k)
  520 p(k)=2.0*bb*a(k)
      p(2)=p(2)+a(1)*2.0*aa
      jm1=j-1
      do 525 k=1,jm1
  525 p(k)=p(k)+q(k)+aa*a(k+1)
      jp1=j+1
      do 530 k=3,jp1
  530 p(k)=p(k)+aa*a(k-1)
      if(j.eq.nm1) go to 540
      do 535 k=1,j
  535 q(k)=-a(k)
      nf1j=nfcns-1-j
      q(1)=q(1)+alpha(nf1j)
  540 continue
      do 543 j=1,nfcns
  543 alpha(j)=p(j)
  545 continue
      if(nfcns.gt.3) return
      alpha(nfcns+1)=0.0
      alpha(nfcns+2)=0.0
      return
      end
c
c-----------------------------------------------------------------------
c function: d
c   function to calculate the lagrange interpolation
c   coefficients for use in the function gee.
c-----------------------------------------------------------------------
c
      double precision function d(k,n,m)
      common pi2,ad,dev,x,y,grid,des,wt,alpha,iext,nfcns,ngrid
      dimension iext(66),ad(66),alpha(66),x(66),y(66)
      dimension des(1045),grid(1045),wt(1045)
      double precision ad,dev,x,y
      double precision q
      double precision pi2
      d=1.0
      q=x(k)
      do 3 l=1,m
      do 2 j=l,n,m
      if(j-k)1,2,1
    1 d=2.0*d*(q-x(j))
    2 continue
    3 continue
      d=1.0/d
      return
      end
c
c-----------------------------------------------------------------------
c function: gee
c   function to evaluate the frequency response using the
c   lagrange interpolation formula in the barycentric form
c-----------------------------------------------------------------------
c
      double precision function gee(k,n)
      common pi2,ad,dev,x,y,grid,des,wt,alpha,iext,nfcns,ngrid
      dimension iext(66),ad(66),alpha(66),x(66),y(66)
      dimension des(1045),grid(1045),wt(1045)
      double precision p,c,d,xf
      double precision pi2
      double precision ad,dev,x,y
      p=0.0
      xf=grid(k)
      xf=dcos(pi2*xf)
      d=0.0
      do 1 j=1,n
      c=xf-x(j)
      c=ad(j)/c
      d=d+c
    1 p=p+c*y(j)
      gee=p/d
      return
      end
c
c-----------------------------------------------------------------------
c subroutine: ouch
c   writes an error message when the algorithm fails to
c   converge.  there seem to be two conditions under which
c   the algorithm fails to converge: (1) the initial
c   guess for the extremal frequencies is so poor that
c   the exchange iteration cannot get started, or
c   (2) near the termination of a correct design,
c   the deviation decreases due to rounding errors
c   and the program stops.  in this latter case the
c   filter design is probably acceptable, but should
c   be checked by computing a frequency response.
c-----------------------------------------------------------------------
c
      subroutine ouch
      common /oops/niter,iout
      write(iout,1)niter
    1 format(44h ************ failure to converge **********/
     141h0probable cause is machine rounding error/
     223h0number of iterations =,i4/
     339h0if the number of iterations exceeds 3,/
     462h0the design may be correct, but should be verified with an fft)
      return
      end
