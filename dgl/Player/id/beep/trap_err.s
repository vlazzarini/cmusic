
	.data
	.text
	.globl	_bus_error
	.globl  _flag_error
_bus_error:
	link	a6,#-LF12	|space for 4 registers plus save a6 and link
	moveml	#LS12,sp@	| save registers a0, a1, d0, d1
	movl	a6@(6),sp@-	| adresse qui a provoque' l'erreur:
	movl	a6@(14),sp@-	| charger PC = adr instr
	.data1
L15:
	.ascii	"instr adresse: 0x%x acces a adresse 0x%x inexistante\12\0"
	.text
	pea	L15
	jbsr	_printf
	addw	#12,sp
	moveml	a6@(-LF12),#LS12| restauration des registres
	unlk	a6
	addqw	#8,sp		| restauration de la pile
				| ici, il faudrait tester l'instruction
				| qui a provoque' l'erreur
	addql	#2,sp@(2)	| aligner PC sur la prochaine instruction
				| ok si 4 bytes instruction
	addqw	#1,_flag_error  | pour signaler l'erreur
	rte
   LF12 = 16		| space to save the 4 registers
	LS12 = 0x0303	|save d0,d1,a0,a1

