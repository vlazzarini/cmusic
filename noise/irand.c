irand(lb, ub)
	int             lb, ub;
{
	register double flb, fub, fr;
	double          frand();
	register int    ir;

	flb = lb;
	fub = ub;
	fr = frand(flb, fub);
	ir = fr;
	return (ir);
}
