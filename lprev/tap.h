/*
 * /1* 7-tap delay line parameters *1/
 *     /1* tap delay times NOTE: array must be terminated with a -1.0 *1/
 * #define TAPDLYS {0,.0199,.0354,.0389,.0414,.0699,.0796,-1.0}
 * 
 * /1* 7-tap delay gains *1/
 *     /1* NOTE: must be same len as TAPDLYS, but isn't terminated with a -1 *1/
 * #define TAPGAINS {1.0, 0.99, .818, .635, .719, .267, .242}
 */

/* 19-tap delay line parameters */
#define TAPDLYS {0,.0043,.0215,.0225,.0268,.0270,.0298,.0458,.0485,.0572, \
	.0587,.0595,.0612, .0707,.0708,.0726,.0741,.0753,.0797,-1.0}

/* 19-tap delay gains */
#define TAPGAINS {1.0,.841,.504,.491,.379,.380,.346,.289, \
	.272,.192,.193,.217,.181,.180,.181,.176,.142,.167,.134}

