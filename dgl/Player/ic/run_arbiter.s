LL0:
	.data
	.comm	_Pdebug,0x4
	.text
|#PROC# 04
	.globl	_run_arbiter
_run_arbiter:
	link	a6,#0
	addl	#-LF74,sp
	moveml	#LS74,sp@
	movl	a5@(38),d7
	movl	a5@(30),d6
	movl	a5@(34),d5
	movl	#1,_Pdebug
L78:
	jbsr	_get_wait
	movl	d0,a5
	tstl	d0
	jne	L79
	jra		L76
L79:
	.data1
L81:
	.ascii	"ptime=%d, beg=%d, dur=%d, Now=%d\12\0"
	.text
	movl	_Now,sp@-
	movl	a5@(34),sp@-
	movl	a5@(30),sp@-
	movl	a5@(38),sp@-
	pea	L81
	jbsr	_printf
	lea	sp@(20),sp
	movb	a5@(42),d0
	andb	#0x1c,d0
	cmpb	#8,d0
	jls	L82
	tstl	_Pdebug
	jeq	L83
	.data1
L84:
	.ascii	"run_arbiter\72 runstat for %s\72%x = %d\12\0"
	.text
	moveq	#0,d0
	movb	a5@(42),d0
	lsrl	#2,d0
	andb	#0x7,d0
	movl	d0,sp@-
	pea	a5@
	movl	a5@(8),sp@-
	pea	L84
	jbsr	_printf
	lea	sp@(16),sp
L83:
	jra		L77
L82:
	btst	#7,a5@(42)
	jne	L10001
	movl	a5@(30),d0
	addl	a5@(34),d0
	movl	a5@(38),d1
	cmpl	d0,d1
	jgt	L10000
	jra	L85
L10001:
	movl	a5@(30),d0
	addl	a5@(34),d0
	movl	a5@(38),d1
	cmpl	d0,d1
	jlt	L85
L10000:
	tstl	_Pdebug
	jeq	L86
	.data1
L87:
	.ascii	"run_arbiter\72 killing %s\72%x\12\0"
	.text
	pea	a5@
	movl	a5@(8),sp@-
	pea	L87
	jbsr	_printf
	lea	sp@(12),sp
L86:
	pea	a5@
	jbsr	_Kill
	addqw	#4,sp
	jra		L77
L85:
	movl	a5@(38),d0
	cmpl	_Now,d0
	jle	L89
	movl	a5@(38),_Now
	jra		L90
L89:
	movl	_Now,a5@(38)
L90:
	movl	a5,_Self
	andb	#0xffffffe3,a5@(42)
	orb	#8,a5@(42)
	tstl	_Pdebug
	jeq	L91
	.data1
L92:
	.ascii	"run_arbiter\72 %d\72\11running %s\72%x\12\0"
	.text
	movl	_Self,sp@-
	movl	a5@(8),sp@-
	movl	_Now,sp@-
	pea	L92
	jbsr	_printf
	lea	sp@(16),sp
L91:
	movl	_Self,sp@-
	movl	a5@,a0
	jsr	a0@
	addqw	#4,sp
	movl	d0,a6@(-48)
	movl	a6@(-48),a0
	movl	#10,d0
	lea	a6@(-44),a1    | left tempval
L10002:	movl	a0@+,a1@+
	dbra	d0,L10002
	bset	#5,a5@(42)
	movb	a5@(42),d0
	andb	#0x1c,d0
	cmpb	#8,d0
	jne	L93
	tstl	_Out_proc
	jeq	L94
	pea	a5@
	movl	_Out_proc,a0
	jsr	a0@
	addqw	#4,sp
L94:
	andb	#0xffffffe3,a5@(42)
	orb	#4,a5@(42)
L93:
	clrl	_Self
L77:
	jra	L78
L76:
	jra	LE74
LE74:
	moveml	a6@(-LF74),#LS74
	unlk	a6
	rts
   LF74 = 64
	LS74 = 0x20e0
	LP74 =	28
	.data
