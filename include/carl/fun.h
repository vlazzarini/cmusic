struct fun_st
{
	int fun_id;
	float	max_x, max_y, min_x, min_y;
	struct fun_st *fun_next, *fun_last;
	float *x;
	float *y;
};

typedef struct fun_st FUN;
extern FUN *rautoxfun();
