c  Input and output subroutines for fir.f
c
c  Gather iin	data for fir.f (program for designing
c  optimum FIR	digital filters). 
c
c  jos:	12-aug-80
c  frm:	18-dec-80
c
	subroutine input(nfilt,nbands,jtype,lgrid,jb,edge,fx,wtx,nfmax,nbmax)
	dimension edge(jb),fx(nbands),wtx(nbands)
c
	iout=6
	iin=5
c lgrid is usually an input parameter. Set larger when convergence fails. 
	lgrid = 16

	write(iout,10)
10	format(/' Finite Impulse Response (FIR) digital filter design program')

15	write(iout,20)
20	format(/'Filter Order (number of coefficients):')
  	read(iin,25)nfilt
25	format(i6)
	if(nfilt.le.0)stop
	if(nfilt.gt.nfmax)then
	write(iout,'("Sorry, the maximum filter order is ",i5)')nfmax
	go to 15 
	end if
30	write(iout,40)
40	format(/'Enter number of frequency bands (pass- and stop-bands)(=2):')
	read(iin,25)nbands
	if(nbands.gt.nbmax)then
	write(iout,'("Sorry, the maximum number of bands is ",i5)')nbmax
	go to 30
	end if
	if(nbands.le.0) then
	nbands=2
	write(iout,'("Number of bands set to ",i3)')nbands
	end if

	write(iout,50)
50	format(/'    Filter type: 1 - LP, HP, BP (default) 2 - Differentiator 3 - Hilbert transformer')
	read(iin,25)jtype
	if(jtype.le.0 .or. jtype.gt.3)then
	jtype=1	
	end if
	write(iout,'(" Type ",i1," selected")')jtype

	jb=2*nbands

	write(iout,'(/"Praise Fortran! You must now include DECIMAL POINTS in all remaining values.")')
	write(iout,'("Also, use COMMAS to separate numbers")')

     	write(iout,'(/"Enter sampling rate in Hz (default is 1.0)")')
	srate = 0.
 	read(iin,'(g20.10)')srate
	if(srate.le.0.)srate=1.0
	write(iout,'(" Sampling rate set to ",f12.3)')srate	

65	write(iout,70)jb
70	format(/'Enter the ',i6,' filter band edges (nbands*2) in Hz')
        write(iout,'("For example, an srate/4 lowpass (2 bands) is")')
        write(iout,'(4(f7.3,","))') 0., srate*0.23, srate*0.25, srate*0.5
	if(jb.gt.10)write(iout,74)
74	format('    Limit entries to 10 per line')
	read(iin,75)(edge(j),j=1,jb)
75	format(10f20.10)
	sum=0.0
	do 100 i=1,jb
100	sum=sum+abs(edge(i))
	if(sum.eq.0.0) then
	do 110 i=1,jb
110	edge(i)=float(i-1)/float(i)
   	edge(jb)=1
	srate = 2
	write(iout,'(" (Band-edges are nominally spaced)")')
	end if
	if(edge(jb).eq.0.0) go to 65
c  Normalize all frequencies to lie between 0 and .5
	do 120 j=1,jb
120	edge(j)=edge(j)/srate
	write(iout,'(" Band-edge frequencies set to")')
	write(iout,'(10f6.2)')(edge(i), i=1,jb)

	if(jtype.eq.2) then
	write(iout,'(/"Enter slope of frequency response in each band (=1)")')
	else 
	write(iout,'(/"Enter desired function across the ",i3,"  bands")')nbands
	write(iout,'("  (e.g. 0,1.=highpass, 1.,0=lowpass, 0,1.,0=highpass)")')
	end if
	if(nbands.gt.10)write(iout,74)
	read(iin,75)(fx(j),j=1,nbands)
c  desired fn in fx (eg 0,1 for hipass, 1,0=lopass, 0,1,0 = bpass
	sum=0.0
	do 140 i=1,nbands
140	sum=sum+abs(fx(i))
	if(sum.eq.0) then
c  1,0,1,0,..., i.e., pass,stop,pass,stop,...
	do 150 i=1,nbands
150	fx(i)=(nbands - i + 1)*(1.-(-1)**i)/2./nbands
	end if
	write(iout,'(" Filter Band Gains set to")')
	write(iout,'(10f6.1)') (fx(i), i=1,nbands)

	write(iout,'(/"Enter desired relative weighting across bands:")')
	if(nbands.gt.10)write(iout,74)
	read(iin,75)(wtx(j),j=1,nbands)
	sum=0.0
	do 170 i=1,nbands
170	sum=sum+abs(wtx(i))
	if (sum.eq.0.) then
	do 180 i=1,nbands
180	wtx(i)=10.0
        wtx(1)=1.0
	write(iout,'(" (Error ripple in 1st band will be 10 times larger than rest)")')
	end if
	write(iout,'(" Band weighting set to")')
	write(iout,'(10f6.1)') (wtx(i), i=1,nbands)
c 1,1	means equal wt, 10,1 means first band is more ideal (10% ripple of 2nd)
	return
	end
c
c	OUTPUT
c
	subroutine putfir(nfilt,neg,h,jtype,lgrid,nbands,edge,fx,wtx,deviat)
c write out FIR filter coefficients from  fir.f and document design
	dimension h(*),hh(512), edge(*), fx(*), wtx(*), deviat(*)
	iof=nfilt/2
	nfcns=(nfilt+1)/2
	if(neg.ne.0)then
	do 10 i=1,nfcns 
10	hh(i+iof)=-h(nfcns-i+1)
	else
	do 30 i=1,nfcns 
30	hh(i+iof)=h(nfcns-i+1)
	end if
	do 20 i=1,iof
20	hh(i)=h(i)
	call outfil(lout)
 	write(lout,40)jtype, lgrid, nbands
40	format('# Say help filterfile for info on how to use this filter',/,
     &  '# FIR tapped delay line filter. Design specs to fir follow:',/,
     &  '# Type=',i2,', grid density=',i3,', number of bands=',i3,/,
     &  '# Band-edges follow:')
	write(lout,41)(edge(i),i=1,2*nbands)
41	format('# ',10f7.4)
	write(lout,'("# Desired function across bands follows:")')
42	format('# ',10f7.2)
	write(lout,42)(fx(i),i=1,nbands)
	write(lout,'("# Weighting function across bands follows:")')
	write(lout,42)(wtx(i),i=1,nbands)
	write(lout,'("# Deviation of each band in dB follows:")')
	write(lout,46)(deviat(i),i=1,nbands)
46	format('# ',5f14.7)
c Write out the filter itself
	write(lout,44)nfilt
44     format('filter;',/,'NIcoeffs = ',i6,';',/,'Icoeffs = ',/)
	call outarr(lout,hh,nfilt)
	close(lout)
	return
	end

	subroutine putarr(x,nx)
	dimension x(nx)
	call outfil(lout)
	call outarr(lout,x,nx)
	close(lout)
	return
	end
c     
	subroutine outfil(lout)
	character*15 outnam
	character*1 reply
	logical exists
1	write(6,'("Output filename:")')
	read(5,"(a)")outnam
	lout=1
	if(outnam.lt.'A'.or. outnam.gt.'z')then
	outnam = 'test.flt'
	end if
	inquire(file=outnam,exist=exists)
	if(exists)then 
	write(6,'(a," already exists. Replace?")')outnam
	read(5,"(a)")reply
	if(reply.ne.'y'.and.reply.ne.'Y') go to 1
	open(lout,file=outnam)
	close(lout, status = 'delete')
	end if
 	write(6,30)outnam
30 	format('Writing: ',a)
	open(lout,file=outnam,status='new')
	return
	end

	subroutine outarr(lout,x,nx)
	dimension x(*)
c	write(6,'("x(i)=",g20.10)')    (x(i),i=1,nx)
	write(lout,*)(x(i),i=1,nx)
	return
	end
