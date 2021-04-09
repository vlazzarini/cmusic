|	/* %M%	%I%	(CARL)	%G%	%U% */

| interrupt service routine for MPU
| level three
| version dgl
| simialr to mpu_asint, except that it calls mpu_cint_dgl

	.globl _mpu_reg		| base of the adress table
MPU_NB=4			| max number of MPUs

DRR=0x40
DSR=0x80

LL0:
	.data
	.text
|#PROC# 04
	.comm	_which_mpu,4	| nb of MPU which interruptedn
	.globl	_mpu_asint_dgl	| assembly routine callled upon interrupt
	.globl	_mpu_cint_dgl	| C routine calles by mpu_asint

|	.globl	_a0
|	.globl	_a1

_mpu_asint_dgl:
	link	a6,#-LF12		| set up stack and save d0, d1, a0, a1
	moveml	#LS12,sp@

| search for which MPU caused the interrupt
	lea 	_mpu_reg,a1
	clrl	d1		| to hold MPU nb
loop:
	movl	a1@,a0		| load MPU address
	cmpl	#0,a0	
	beq	iznogud		| This MPU isn't hooked up
	
	movb	a0@(1),d0	| read status register
	andb	#DSR,d0
	beq	cont		| this is the one

iznogud:
	addqw	#1,d1		| control the number of MPus
	cmpw	#MPU_NB,d1
	beq	getout		| weird !! spurious interrupt
	addqw	#2,a1		| next MPU
	bra	loop
cont:
	movl	d1,_which_mpu	| MPU nb
	movb	a0@,d0		| read first data byte
	movl	d0,sp@-		| push on stack
	jbsr	_mpu_cint_dgl	| jump to C routine
	addqw	#4,sp
getout:
	moveml	a6@(-LF12),#LS12
	unlk	a6
	rte

	
   LF12 = 16		| space to save the 4 registers
	LS12 = 0x0303	|save d0,d1,a0,a1

