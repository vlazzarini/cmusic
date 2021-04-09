C-----------------------------------------------------------------------
C SUBROUTINE:  KAISER
C KAISER WINDOW
C-----------------------------------------------------------------------
C
      SUBROUTINE KAISER(NF, W, N, IEO, BETA)
C
C   NF = FILTER LENGTH IN SAMPLES
C    W = WINDOW ARRAY OF SIZE N
C    N = FILTER HALF LENGTH=(NF+1)/2
C  IEO = EVEN ODD INDICATOR--IEO=0 IF NF EVEN
C BETA = PARAMETER OF KAISER WINDOW
C
      DIMENSION W(1)
      REAL INO
      BES = INO(BETA)
      XIND = FLOAT(NF-1)*FLOAT(NF-1)
      DO 10 I=1,N
	XI = I - 1
	IF (IEO.EQ.0) XI = XI + 0.5
	XI = 4.*XI*XI
	W(I) = INO(BETA*SQRT(1.-XI/XIND))
	W(I) = W(I)/BES
  10  CONTINUE
      RETURN
      END
C
C-----------------------------------------------------------------------
C FUNCTION:  INO
C BESSEL FUNCTION FOR KAISER WINDOW
C-----------------------------------------------------------------------
C
      REAL FUNCTION INO(X)
      Y = X/2.
      T = 1.E-08
      E = 1.
      DE = 1.
      DO 10 I=1,25
	XI = I
	DE = DE*Y/XI
	SDE = DE*DE
	E = E + SDE
	IF (E*T-SDE) 10, 10, 20
  10  CONTINUE
  20  INO = E
      RETURN
      END
