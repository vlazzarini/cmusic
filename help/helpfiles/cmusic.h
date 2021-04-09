/* cmusic.h - standard macro definitions for cmusic scores */
#define QUAD(In,Out)\
set quad;\
set space = Out/2,Out/2 -Out/2,Out/2 -Out/2,-Out/2 Out/2,-Out/2 ;\
set room = In/2,In/2 -In/2,In/2 -In/2,-In/2 In/2,-In/2 ;\
set speakers = In/2,In/2 -In/2,In/2 -In/2,-In/2 In/2,-In/2 ;\
set revscale = .1 ;\
set t60 = (Out/10)^.5 
#define STEREO(In,Out)\
set stereo;\
set space = Out/2,Out/2 -Out/2,Out/2 -Out/2,-Out/2 Out/2,-Out/2 ;\
set room = In/2,In/2 -In/2,In/2 -In/2,-In/2 In/2,-In/2 ;\
set speakers = In/2,In/2 -In/2,In/2 ;\
set revscale = .1 ;\
set t60 = (Out/10)^.5 
/*
 * unit generator statement abbreviations
 */
#define SPACE(b,n) space b n d d d d d d d d d d d d
#define NRES(out,in,gain,cf,bw) nres out in gain cf bw d d d d d d d d d d
#define FLTDELAY(b) fltdelay b d d d d d d d d d d d d d d d
#define AIRABSORB(OUT,IN,X,Y) airabsorb OUT IN X Y d d d d d d d d d d
/*
 * waveform components
 */
#define PLS(num) num,1,0 
#define SAW(num) num,1/num,0 
#define TRI(num) num,1/num^2,0 
/*
 * useful signal waveforms
 */
#define SINE(func) gen p2 gen5 func 1 1 0
#define COS(func)  gen p2 gen5 func 1 1 90Deg
#define TRIANGLE(func) gen p2 gen5 func TRI(1) TRI(3) TRI(5) TRI(7); NORM(func)
#define SQUARE(func) gen p2 gen5 func SAW(1) SAW(3) SAW(5) SAW(7); NORM(func)
#define SAWTOOTH(func) gen p2 gen5 func SAW(1) SAW(2) SAW(3) SAW(4) SAW(5) \
SAW(6) SAW(7) SAW(8); NORM(func)
#define PULSE(func) gen p2 gen5 func PLS(1) PLS(2) PLS(3) PLS(4) PLS(5) \
PLS(6) PLS(7) PLS(8); NORM(func)
/*
 * use envelope waveforms
 */
#define ENV(func) gen p2 gen4 func 0,0 -1 .1,1 -1 .8,.5 -1 1,0
#define SLOWENV(func) gen p2 gen4 func 0,0 -1 1/3,1 -1 2/3,.5 -1 1,0
#define PLUCKENV(func) gen p2 gen4 func 0,0 -1 .005,1 -2 1,0
/*
 * gen statement abbreviations
 */
#define GEN0(func) gen p2 gen0 func 
#define NORM(func) gen p2 gen0 func 1
#define GEN1(func) gen p2 gen1 func 
#define GEN2(func) gen p2 gen2 func 
#define GEN3(func) gen p2 gen3 func 
#define GEN4(func) gen p2 gen4 func 
#define GEN5(func) gen p2 gen5 func 
#define GEN6(func) gen p2 gen6 func 
#define CHUBBY(func) gen p2 chubby func 
#define CSPLINE(func) gen p2 cspline func 
#define GENRAW(func) gen p2 genraw func 
#define QUADGEN(func) gen p2 quad func 
#define SHEPENV(func) gen p2 shepenv func 
/*
 * generates random number between low and high
 */
#define RAND(low,high)(rand(high-low)+low)
/*
 * general period definition
 */
#define P (p4sec)
/*
 * pitch reference = middle C
 */
#define REF (220*2^(3/12))
/*
 * 12-tone temperament frequencies
 */
#define FR(pitch,oct)(REF*2^(oct)*2^(pitch/12))
/*
 * tempered scale pitch classes (0 octave = middle C up to B)
 */
#define C(oct)(FR(0,oct)Hz)
#define Cs(oct)(FR(1,oct)Hz)
#define Df(oct)(FR(1,oct)Hz)
#define D(oct)(FR(2,oct)Hz)
#define Ds(oct)(FR(3,oct)Hz)
#define Ef(oct)(FR(3,oct)Hz)
#define E(oct)(FR(4,oct)Hz)
#define F(oct)(FR(5,oct)Hz)
#define Fs(oct)(FR(6,oct)Hz)
#define Gf(oct)(FR(6,oct)Hz)
#define G(oct)(FR(7,oct)Hz)
#define Gs(oct)(FR(8,oct)Hz)
#define Af(oct)(FR(8,oct)Hz)
#define A(oct)(FR(9,oct)Hz)
#define As(oct)(FR(10,oct)Hz)
#define Bf(oct)(FR(10,oct)Hz)
#define B(oct)(FR(11,oct)Hz)
/*
 * just intervals for major scale
 */
#define P1(oct)(2^(oct))
#define M2(oct)(2^(oct)*9/8)
#define M3(oct)(2^(oct)*5/4)
#define P4(oct)(2^(oct)*4/3)
#define P5(oct)(2^(oct)*3/2)
#define M6(oct)(2^(oct)*5/3)
#define M7(oct)(2^(oct)*15/8)
#define TT(oct)(2^(oct)*2^.5)
/*
 * just intervals for minor scale
 * major second is different in minor scale
 */
#define mM2(oct)(2^(oct)*10/9)
#define m3(oct)(2^(oct)*6/5)
#define m6(oct)(2^(oct)*8/5)
#define m7(oct)(2^(oct)*9/5)
/*
 * misc functions
 */
#define xy(distance, direction) distance*cos(direction) distance*sin(direction)
#define LOG10(x)(ln(x)/ln(10))
#define LOG2(x)(ln(x)/ln(2))
/*
 * map x in range [min1,max1] linearly to range [min2,max2]
 */
#define MAP(x,min1,max1,min2,max2)((x)-(min1))*((max2)-(min2))/((max1)-(min1))+(min2)
/*
 * vowel formant frequencies and amplitudes (.A) for men (M.), women (W.) and
 * children (C.) per Peterson & Barney (1952)
 */
#define M_IY1 (270Hz)
#define M_IY2 (2290Hz)
#define M_IY3 (3010Hz)

#define W_IY1 (310Hz)
#define W_IY2 (2790Hz)
#define W_IY3 (3310Hz)

#define C_IY1 (370Hz)
#define C_IY2 (3200Hz)
#define C_IY3 (3730Hz)

#define IY1_A (-4dB)
#define IY2_A (-24dB)
#define IY3_A (-28dB)

#define M_I1 (390Hz)
#define M_I2 (1990Hz)
#define M_I3 (2550Hz)

#define W_I1 (430Hz)
#define W_I2 (2480Hz)
#define W_I3 (3070Hz)

#define C_I1 (530Hz)
#define C_I2 (2730Hz)
#define C_I3 (3600Hz)

#define I1_A (-3dB)
#define I2_A (-23dB)
#define I3_A (-27dB)

#define M_E1 (530Hz)
#define M_E2 (1840Hz)
#define M_E3 (2480Hz)

#define W_E1 (610Hz)
#define W_E2 (2330Hz)
#define W_E3 (2990Hz)

#define C_E1 (690Hz)
#define C_E2 (2610Hz)
#define C_E3 (3570Hz)

#define E1_A (-2dB)
#define E2_A (-17dB)
#define E3_A (-24dB)

#define M_AE1 (660Hz)
#define M_AE2 (1720Hz)
#define M_AE3 (2410Hz)

#define W_AE1 (860Hz)
#define W_AE2 (2050Hz)
#define W_AE3 (2850Hz)

#define C_AE1 (1010Hz)
#define C_AE2 (2320Hz)
#define C_AE3 (3320Hz)

#define AE1_A (-1dB)
#define AE2_A (-12dB)
#define AE3_A (-22dB)

#define M_UH1 (640Hz)
#define M_UH2 (1190Hz)
#define M_UH3 (2390Hz)

#define W_UH1 (760Hz)
#define W_UH2 (1400Hz)
#define W_UH3 (2780Hz)

#define C_UH1 (850Hz)
#define C_UH2 (1590Hz)
#define C_UH3 (3360Hz)

#define UH1_A (-1dB)
#define UH2_A (-10dB)
#define UH3_A (-27dB)

#define M_A1 (730Hz)
#define M_A2 (1090Hz)
#define M_A3 (2440Hz)

#define W_A1 (850Hz)
#define W_A2 (1220Hz)
#define W_A3 (2810Hz)

#define C_A1 (1030Hz)
#define C_A2 (1370Hz)
#define C_A3 (3170Hz)

#define A1_A (-1dB)
#define A2_A (-5dB)
#define A3_A (-28dB)

#define M_OW1 (570Hz)
#define M_OW2 (840Hz)
#define M_OW3 (2410Hz)

#define W_OW1 (590Hz)
#define W_OW2 (920Hz)
#define W_OW3 (2710Hz)

#define C_OW1 (680Hz)
#define C_OW2 (1060Hz)
#define C_OW3 (3180Hz)

#define OW1_A (0dB)
#define OW2_A (-7dB)
#define OW3_A (-34dB)

#define M_U1 (440Hz)
#define M_U2 (1020Hz)
#define M_U3 (2240Hz)

#define W_U1 (470Hz)
#define W_U2 (1160Hz)
#define W_U3 (2680Hz)

#define C_U1 (560Hz)
#define C_U2 (1410Hz)
#define C_U3 (3310Hz)

#define U1_A (-1dB)
#define U2_A (-12dB)
#define U3_A (-34dB)

#define M_OO1 (300Hz)
#define M_OO2 (870Hz)
#define M_OO3 (2240Hz)

#define W_OO1 (370Hz)
#define W_OO2 (950Hz)
#define W_OO3 (2670Hz)

#define C_OO1 (430Hz)
#define C_OO2 (1170Hz)
#define C_OO3 (3260Hz)

#define OO1_A (-3dB)
#define OO2_A (-19dB)
#define OO3_A (-43dB)

#define M_ER1 (490Hz)
#define M_ER2 (1350Hz)
#define M_ER3 (1690Hz)

#define W_ER1 (500Hz)
#define W_ER2 (1640Hz)
#define W_ER3 (1960Hz)

#define C_ER1 (560Hz)
#define C_ER2 (1820Hz)
#define C_ER3 (2160Hz)

#define ER1_A (-5dB)
#define ER2_A (-15dB)
#define ER3_A (-20dB)

#define MF1BW (50Hz)
#define MF2BW (50Hz)
#define MF3BW (50Hz)

#define WF1BW (50Hz)
#define WF2BW (50Hz)
#define WF3BW (100Hz)

#define CF1BW (50Hz)
#define CF2BW (100Hz)
#define CF3BW (150Hz)
