| routines read and write for mpu
| the call from C programms is mpuwrite(data,mpu_number)
| the data byte to write is in a6@(11) mpu nb in a6@(12)
|*******************************************************************
| Hardware requirement for the MPU register address: 
| data register at MPU0_DATA + no MPU * 2
| status or cmd register at
| MPU0_DATA + no MPU * 2 + 1 = MPU0_CMD + no MPU * 2
|*******************************************************************
	.globl _mpu_reg		| base of the address table
	.globl _which_mpu	| number of the mpu which interrupted
				| for quick write mpuqwrite
DRR=0x40
DSR=0x80
AKN= 0xfe
	.data
	.text
|#PROC# 04
	.globl	_mpuwrite
_mpuwrite:
	link	a6,#0
	lea	_mpu_reg,a1
	movl	a6@(12),d0		| MPU nb must be word
	asll	#3,d0			| MPU nb * 8
	addl	d0,a1			| add offset
	movl	a1@,a0			| data register address in a0
write:
    	movb	a0@(1),d0		| read MPU status
	andb	#DRR,d0
	jne	write

	movb	a6@(11),a0@		| write byte data
	unlk	a6
	rts
	.data
	.text

| mpu quick write. the mpu number is in which_mpu rather than on the stack
| this works only if called from the interrupt driver
| call from C: mpuqwrite(data) ;

	.globl	_mpuqwrite
_mpuqwrite:
	link	a6,#0

	lea	_mpu_reg,a1		| load address table base address
	movl	_which_mpu,d0		| MPU nb must be word
	asll	#3,d0			| MPU nb * 8
	addl	d0,a1			| add offset
	movl	a1@,a0			| data register address in a0
qwrite:
    	movb	a0@(1),d0		| read MPU status
	andb	#DRR,d0
	jne	write

	movb	a6@(11),a0@		| write byte data
	unlk	a6
	rts
	.data
	.text

| tha data byte is in d0
| the call is data = mpuread(mpu_nb) ;
| with char data, mpuread(); int mpu_nb ;
| mpu_nb is in a6@(8)

	.globl	_mpuread
_mpuread:
	link	a6,#0

	lea	_mpu_reg,a1
	movl	a6@(8),d0		| MPU nb must be integer
	asll	#3,d0			| MPU nb * 8
	addl	d0,a1			| add offset
	movl	a1@,a0			| data register address in a0
read:
	movb	a0@(1),d0		| read MPU status
	andb	#DSR,d0
	jne	read

	clrl	d0
	movb	a0@,d0			| return data byte

	unlk	a6
	rts
	.data
	.text
|---------------------------------------------------------
|	the call from C programs is mpucmd(cmd,mpu_nb)
|	char cmd; int mpu_nb;
|	the cmd byte is in a6@(11)

	.globl	_mpuwcmd
_mpuwcmd:
	link	a6,#0
	addl	#-LF23,sp
	moveml	#LS23,sp@

	lea	_mpu_reg,a1		| compute address in table
	movl	a6@(12),d0		| MPU nb must be integer
	asll	#3,d0			| MPU nb * 8
	addl	d0,a1			| add offset
	movl	a1@,a0			| data register address in a0
wcmd:
	movb	a0@(1),d0		| test status

	andb	#DRR,d0
	bne	wcmd

	orw 	#0x0300,sr	| desable intr	3

	movb	a6@(11),a0@(1)	|	write the command

akn:
	movb	a0@(1),d0	|	wait for aklnoledge 0xfe
	andb	#DSR,d0
	jne	akn

	movb	a0@,d7		|	read data byte from MPU
	cmpb	#AKN,d7
	beq	fin
|
|	if not akn juimpo to intr routine
	extl	d7
	movl	d7,sp@-
	jbsr	_mpu_cint_dgl
	addqw	#4,sp
	jra	akn
fin:
	andw #0x2000,sr		|enable intr

	moveml	a6@(-LF23),#LS23
	unlk	a6
	rts
   LF23 = 4			| one register = four bytes
	LS23 = 0x80		| mask for d0
	LP23 =	16
	.data
