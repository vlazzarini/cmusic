| library for routines not available in C
	.data
	.comm	_savsr,0x2
	.text
	.globl	_savsr
	.globl	_desint
| to desable interrupts
| call from C: desint(no);int no;

_desint:
	link	a6,#0
	movl	a6@(8),d0	|numero de l'it
|cludge because the instr or on the SR works only with absolute value
	cmpl	#1,d0
	beq	un
	cmpl	#2,d0
	beq	deux
	cmpl	#3,d0
	beq	trois
	cmpl	#4,d0
	beq	quatre
	cmpl	#5,d0
	beq	cinq
	cmpl	#6,d0
	beq	six
	cmpl	#7,d0
	beq	sept

	movw	sr,_savsr
un:	orw	#0x0100,sr
	jra	exit
deux:	orw	#0x0200,sr
	jra	exit
trois:	orw	#0x0300,sr
	jra	exit
quatre:	orw	#0x0400,sr
	jra	exit
cinq:	orw	#0x0500,sr
	jra	exit
six:	orw	#0x0600,sr
	jra	exit
sept:	orw	#0x0700,sr
exit:
	unlk	a6
	rts
| to enable interrupts
| call from C: enint(no);int no ;

	.globl	_enint
_enint:
	link	a6,#0
	movl	a6@(8),d0
	cmpl	#1,d0
	beq	uns
	cmpl	#2,d0
	beq	deuxs
	cmpl	#3,d0
	beq	troiss
	cmpl	#4,d0
	beq	quatres
	cmpl	#5,d0
	beq	cinqs
	cmpl	#6,d0
	beq	sixs
	cmpl	#7,d0
	beq	septs

	movw	sr,_savsr
uns:	andw	#0x2600,sr
	jra	exit
deuxs:	andw	#0x2500,sr
	jra	exit
troiss:	andw	#0x2400,sr
	jra	exit
quatres:	andw	#0x2300,sr
	jra	exit
cinqs:	andw	#0x2200,sr
	jra	exit
sixs:	andw	#0x2100,sr
	jra	exit
septs:	andw	#0x2000,sr
exits:
	unlk	a6
	rts

	.globl	_readsr
_readsr:
	link	a6,#0
	movw	sr,d0
	unlk	a6
	rts

	.globl	_desints
	.globl	_enints

_desints:
	movw	sr,_savsr
	orw	#0x0700,sr
	rts
_enints:
	movw	_savsr,sr
	rts
