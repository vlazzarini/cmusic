/*
 * minimalist version of MPU driver	
 * made in France
 * this version suspects that only MPU0 is connected
 * needs liba68.a: libm68.o ckmem.o trap_err.o trap_init.o
 */

# include "xc_mpu.h"	/* secret handy scmulblic	*/

MPU_REG mpu_reg[MPU_NB] = {0};	/* address table	*/

mpu_init()
{
	extern mpu_asint_dgl() ;/* in mpu_cint_dgl.c	*/
	extern test_adr() ;	/* in ckmem.c		*/
	extern enint()	;	/* in libm68.s		*/
	register i ;
	
/* yet only MPU0 is hooked up		*/
	mpu_reg[0].mpu_reg_data = (char *) MPU0_DATA ;
	mpu_reg[0].mpu_reg_cmd = (char *)MPU0_CMD ;

/* make shure the MPUs are there		*/
	test_adr(MPU0_DATA);
	test_adr(MPU0_CMD);

/* No other MPU			*/
	for(i = 1; i < MPU_NB ; i++) {
		mpu_reg[i].mpu_reg_data = (char *) NULL ;
		mpu_reg[i].mpu_reg_cmd = (char *) NULL ;		
	}

	*MPUIT = (int)mpu_asint_dgl  ;/* one assumes that the it is enable*/
	enint(3) ;
	m_mpureset(0) ;
	m_bendoff(0) ;
	m_measoff(0) ;
	m_initthruoff(0) ;

}
