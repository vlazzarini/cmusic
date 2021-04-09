c
c-----------------------------------------------------------------------
c  function:  uni
c  author:    alan m. gross
c             bell laboratories, murray hill, new jersey 07974
c  portable random number generator
c-----------------------------------------------------------------------
c
      function uni(k)
      integer ibyte(4)
      data icseed/0/, itseed/0/, ifcn/1/
c
c  uni is returned as a single real random variate
c  from the uniform distribution 0.0 .le. uni .lt. 1.0 .
c
c  ifcn = 1 implies that icseed, itseed, ibyte, and k are ignored.
c
      uni=r1unif(icseed,itseed,ibyte,ifcn)
      return
      end
      subroutine ranset(icseed,itseed)
      integer ibyte(4)
      data ifcn/0/
c
c  to (re)initialize the uniform random number generator, r1unif
c  (to other than the default initial values).
c
c    icseed is the new seed for congruential generator.
c    itseed is the new seed for tausworthe generator.
c
c  one, but not both, of the new seeds can be zero
c
c
c  ifcn = 0 implies that uni and ibyte are not computed.
c
      uni=r1unif(icseed,itseed,ibyte,ifcn)
      return
      end
      subroutine ranbyt(uni,ibyte)
      dimension ibyte(4)
      data icseed/0/, itseed/0/, ifcn/2/
c
c  uni is returned as a single uniform random variate in uni.
c
c  ibyte is returned with the bits of uni, 8 bits per word.
c  uni=(ibyte(1)*256**3+ibyte(2)*256**2+ibyte(3)*256+ibyte(4))/2**32
c
c  ifcn = 2 implies that icseed and itseed are ignored.
c
      uni=r1unif(icseed,itseed,ibyte,ifcn)
      return
      end
      function r1unif(icseed,itseed,ibyte,ifcn)
c
c  r1unif - output, the uniform random number if ifcn .ne. 0
c  icseed - input, the new congruential seed if ifcn = 0
c  itseed - input, the new tausworthe seed if ifcn = 0
c  ibyte  - output, the bits of r1unif, 8 per word, if ifcn = 2
c  ifcn   - input, = 0 for initialization
c                  = 1 if only the value of r1unif is of interest
c                  = 2 if both r1unif and ibyte are of interest
c
c  this is a portable fortran implementation of uni, a
c  uniform random number generator on (0.0, 1.0) devised
c  by marsaglia, et. al., and included in their package
c  called "super-duper".
c
c  two independent 32 bit generators are maintained internally and
c  updated for each call.
c
c  the first of these is a congruential generator with
c  multiplier 69069 (=16*64**2 + 55*64 + 13).
c
c  the second is a tausworthe or shift-register generator.
c  this generator takes the seed, shifts it right 15 bits, exclusive
c  ors it with itself, shifts the result 17 bits to the left, and
c  exclusive ors the shifted result with itself (not with the
c  original seed).  the output of the procedure is the tausworthe
c  random number and is used as the seed for the next call.
c
c  finally, the output from the two generators is
c  exclusively or-ed together.
c
c  the following program should work on any 16+ bit computer.
c
      logical  first
      integer cseed(6), tseed(32), xor(29), ibyte(4), iscr(5)
      data xor(1)/1/,xor(2)/2/,xor(3)/3/,xor(4)/3/,xor(5)/2/,
     1 xor(6)/1/,xor(7)/4/,xor(8)/5/,xor(9)/6/,xor(10)/7/,xor(11)/5/,
     2 xor(12)/4/,xor(13)/7/,xor(14)/6/,xor(15)/1/,xor(16)/6/,
     3 xor(17)/7/,xor(18)/4/,xor(19)/5/,xor(20)/2/,xor(21)/3/,
     4 xor(22)/7/,xor(23)/6/,xor(24)/5/,xor(25)/4/,xor(26)/3/,
     5 xor(27)/2/,xor(28)/1/,xor(29)/0/
      data first/.true./, jcseed/12345/, jtseed/1073/
c
c  initialize cseed and tseed for portability
c
      data cseed(1)/0/,cseed(2)/0/,cseed(3)/0/,cseed(4)/0/,
     1 cseed(5)/0/,cseed(6)/0/,tseed(1)/0/,tseed(2)/0/,tseed(3)/0/,
     2 tseed(4)/0/,tseed(5)/0/,tseed(6)/0/,tseed(7)/0/,tseed(8)/0/,
     3 tseed(9)/0/,tseed(10)/0/,tseed(11)/0/,tseed(12)/0/,
     4 tseed(13)/0/,tseed(14)/0/,tseed(15)/0/,tseed(16)/0/,
     5 tseed(17)/0/,tseed(18)/0/,tseed(19)/0/,tseed(20)/0/,
     6 tseed(21)/0/,tseed(22)/0/,tseed(23)/0/,tseed(24)/0/,
     7 tseed(25)/0/,tseed(26)/0/,tseed(27)/0/,tseed(28)/0/,
     8 tseed(29)/0/,tseed(30)/0/,tseed(31)/0/,tseed(32)/0/
c
      r1unif=0.0
      if((.not.first) .and. (ifcn.gt.0)) go to 50
      if(ifcn.gt.0) go to 10
c
c     take user values as seeds
c
      jcseed=iabs(icseed)
      jtseed=iabs(itseed)
   10 first=.false.
c
c.....decode seeds
c
      cseed(1)=jcseed
      do 20 i=1,5
      cseed(i+1)=cseed(i)/64
   20 cseed(i)=cseed(i)-cseed(i+1)*64
      cseed(6)=mod(cseed(6),4)
c
c     ensure odd unless zero
c
      if(jcseed.ne.0 .and. mod(cseed(1),2).eq.0) cseed(1)=cseed(1)+1
      tseed(1)=jtseed
      do 30 i=1,11
      tseed(i+1)=tseed(i)/2
   30 tseed(i)=tseed(i)-tseed(i+1)*2
c
c     only use initial value mod 2048
c
      do 40 i=12,32
   40 tseed(i)=0
c
c     ensure odd unless zero
c
      if(jtseed.ne.0) tseed(1)=1
c
c     end of initialization
c
      if(ifcn.eq.0) return
   50 continue
c
c.....tausworthe generator -- shift right 15, then left 17
c
      do 60 i=1,17
   60 tseed(i)=iabs(tseed(i)-tseed(i+15))
      do 70 i=18,32
   70 tseed(i)=iabs(tseed(i)-tseed(i-17))
c
c.....congruential generator -- multiplication in base 64
c
c     multiply base 64
c
      cseed(6)=13*cseed(6)+55*cseed(5)+16*cseed(4)
      cseed(5)=13*cseed(5)+55*cseed(4)+16*cseed(3)
      cseed(4)=13*cseed(4)+55*cseed(3)+16*cseed(2)
      cseed(3)=13*cseed(3)+55*cseed(2)+16*cseed(1)
      cseed(2)=13*cseed(2)+55*cseed(1)
      cseed(1)=13*cseed(1)
      k=-5
      icarry=0
      do 80 i=1,5
      k=k+6
      cseed(i)=cseed(i)+icarry
      icarry=cseed(i)/64
      cseed(i)=cseed(i)-64*icarry
      i2=cseed(i)/8
      i1=cseed(i)-8*i2
      j1=4*tseed(k+2)+tseed(k+1)+tseed(k+1)+tseed(k)
      j2=4*tseed(k+5)+tseed(k+4)+tseed(k+4)+tseed(k+3)
      it1=28
      if(i1.gt.j1) it1=(i1*i1-i1)/2+j1
      if(i1.lt.j1) it1=(j1*j1-j1)/2+i1
      it2=28
      if(i2.gt.j2) it2=(i2*i2-i2)/2+j2
      if(i2.lt.j2) it2=(j2*j2-j2)/2+i2
      iscr(i)=8*xor(it2+1)+xor(it1+1)
   80 r1unif=(r1unif+float(iscr(i)))/64.0
      cseed(6)=mod(cseed(6)+icarry,4)
      j1=tseed(31)+tseed(32)+tseed(32)
      it1=iabs(cseed(6)-j1)
      if((it1.eq.1) .and. (cseed(6)+j1.eq.3)) it1=3
      r1unif=(r1unif+float(it1))/4.0
      if(ifcn.eq.1) return
      ibyte(4)=iscr(1)+mod(iscr(2),4)*64
      ibyte(3)=iscr(2)/4+mod(iscr(3),16)*16
      ibyte(2)=iscr(3)/16+iscr(4)*4
      ibyte(1)=iscr(5)+it1*64
      return
      end
