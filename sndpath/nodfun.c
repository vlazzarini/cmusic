#include "list.h"
#include <stdio.h>
#include <math.h>

/*
 * wfun - write function file name with node list nod.
 * File written is text, with x and y pairs on each line. 
 */

wfun(name, nod, cx, cy, cd, ysin)
	char *name; LIST *nod; int cx, cy; float cd; int ysin;
{
	LIST *p;
	FILE *of, *fopen();
	of = fopen(name, "w");
	if (of == NULL) return(-1);
	for (p = nod; p; p = p->next) fprintf(of, "%6.3f\t%6.3f\n", 
		cd * (p->x - cx), cd * (p->y - cy) * ysin);
	fclose(of);
	return(0);
}

/*
 * rfun - read function file name.  if head != NULL, append the function
 * to the end of it.  returns the head, which it creates if supplied one
 * is NULL.  cx, cy and cd are used to set meter distances of the nodes 
 * from the listener.  Typical values would be cx = cy = 0, cd = 1.0.
 * cd is interpreted in meters.
 */

LIST *rfun(head, name, cx, cy, cd, ysin)
	LIST *head; char *name; float cx, cy, cd; int ysin;
{
	LIST *p, *addnod();
	FILE *i, *fopen();
	float fx=0, fy=0;
	register int r;
	register float n = 0.0;
	i = fopen(name, "r");
	if (i == NULL) return(NULL);
	for (p = head; p; p = p->next) n = p->n + 1.0;	
	while ((r = fscanf(i, "%f%f", &fx, &fy)) != EOF && r == 2)
		{
		head = addnod(head, fy, fx, n, n, cx, cy, cd, ysin);
		n += 1.0;
		}
	fclose(i);
	return(head);
}


rmlist(nod)
	LIST *nod;
{
	LIST *s, *o;
	for (s = nod; s; )
		if (s->next != NULL)
			{ s = s->next; free(s->last); }
		else
			{ free(s); break; }
}


/* 
 * linearly interpolate node between this and the next for x and y 
 * where x,y are screen coordinates.
 */

LIST *ntrpnod(nod, cx, cy, cd)	
	LIST *nod; int cx, cy; float cd;
{
	LIST *newnod, *nxtnod, *t, *mknod();
	if (nod == NULL) return(NULL);
	if (nod->next == NULL) return(NULL);
	else nxtnod = nod->next;
	newnod = mknod();
	newnod->y = ((nxtnod->y - nod->y)/2) + nod->y;
	newnod->fy = -(newnod->y - cy) * cd;
	newnod->x = ((nxtnod->x - nod->x)/2) + nod->x;
	newnod->fx = (newnod->x - cx) * cd;
	newnod->time = ((nxtnod->time - nod->time)/2) + nod->time;
	newnod->n = nxtnod->n;
	for (t = nxtnod; t; t = t->next) t->n += 1.0;
	nod->next = newnod;
	newnod->last = nod;
	newnod->next = nxtnod;
	nxtnod->last = newnod;
	if (velocity(newnod))
		return(NULL);
	return(newnod);
}


LIST *mknod()
{
	return((LIST *) calloc(sizeof (LIST), 1));
}


/*adrnod - returns node with same values of y and x.*/

LIST *adrnod(head, y, x)
	LIST *head; int x, y;
{
	LIST *nod;
	for (nod = head; nod; nod = nod->next)
		if (nod->x == x && nod->y == y)
			return(nod);
	return(NULL);
}


/* putnod - position node to x,y coordinates where x,y are the integer
 * values of a screen location.  0,0 is the upper left hand corner.
 */

putnod(nod, y, x, cx, cy, cd)
	LIST *nod; int y, x, cx, cy; float cd;
{
	if (nod == NULL) return(-1);
	nod->x = x;
	nod->fx = (x - cx) * cd;
	nod->y = y;
	nod->fy = -(y - cy) * cd;
	if (velocity(nod))
		return(-1);
	return(0);
}

mvlist(head, cx, cy, cd, ysign)
	LIST *head; float cx, cy, cd; int ysign;
{
	LIST *n;
	for (n = head; n; n = n->next)
		{
		n->y = cy + ysign * n->fy / cd;
		n->x = cx + n->fx / cd;
		}
}

scalist(head, xscl, yscl, cx, cy, cd, ysign)
	LIST *head; float xscl, yscl, cx, cy, cd; int ysign;
{
	LIST *n;
	for (n = head; n; n = n->next)
		{
		n->fx *= xscl;
		n->fy *= yscl;
		n->y = cy + ysign * n->fy / cd;
		n->x = cx + n->fx / cd;
		if (velocity(n))
			return(-1);
		}
	return(0);
}

offlist(head, xoff, yoff, cx, cy, cd, ysign)
	LIST *head; float xoff, yoff, cx, cy, cd; int ysign;
{
	LIST *n;
	for (n = head; n; n = n->next)
		{
		n->fx += xoff;
		n->fy += yoff;
		n->y = cy + ysign * n->fy / cd;
		n->x = cx + n->fx / cd;
		}
}

float Pi;

float getheta(x, y)
	float x, y;
{
	register float theta;
	theta = atan2(y, x);
	if (theta < 0.0) theta = (Pi + theta) + Pi;
	return(theta);
}

float circlemod(x) 
	float x;
{
	register float twopi = 2.0 * Pi;
	while (x > twopi) x -= twopi;
	while (x < 0) x += twopi;
	return(x);
}

float getdistance(x, y)
	float x, y;
{
	return(sqrt(x*x+y*y));
}

rotlist(head, rot, cx, cy, cd, ysign)
	LIST *head; float rot, cx, cy, cd; int ysign;
{
	LIST *n;
	float theta, D;
	Pi = 4.0 * atan(1.0);
	rot = 2.0 * Pi * rot / 360.0;
	for (n = head; n; n = n->next)
		{
		D = getdistance(n->fx, n->fy);
		theta = getheta(n->fx, n->fy);
		theta = circlemod(theta + rot);
		n->fx = D * cos(theta);
		n->fy = D * sin(theta);
		n->y = cy + ysign * n->fy / cd;
		n->x = cx + n->fx / cd;
		}
}


LIST *endnod;

LIST *addnod(head, y, x, n, time, cx, cy, cd, ysign)
	LIST *head; float y, x, n, time, cx, cy, cd; int ysign;
{
	LIST *newnod, *mknod();
	if (head == NULL)
		newnod = endnod = head = mknod();
	else
		newnod = mknod();
	newnod->y = cy + ysign * y / cd;
	newnod->fy = y;
	newnod->x = cx + x / cd;
	newnod->fx = x;
	newnod->n = n;
	newnod->time = time;
	if (newnod != head) 	/* not starting new list */
		{
		endnod->next = newnod;
		newnod->last = endnod;
		}
	if (velocity(newnod))
		return(NULL);
	endnod = newnod;
	return(head);
}


/*
 * delnod - delete a node from the list.  returns the previous node if
 * possible, else the next node.  if the node is the only one, NULL is
 * returned.  if the node to be deleted is the head of the linked list,
 * assign the next element to be head.  Note that the ADDRESS of the
 * head of the linked list must be passed, not just its value.
 */

LIST *delnod(Hed, nod)
	LIST **Hed, *nod;
{
	LIST *s, *r = NULL;
	if (nod == NULL) return(NULL);
	if (nod->last != NULL)
		nod->last->next = nod->next;
	else
		*Hed = nod->next;	/* jump to new head */
	if (nod->next != NULL)
		nod->next->last = nod->last;
	for (s = nod->next; s; s = s->next) s->n -= 1.0;
	if (velocity(nod->next))
		return(NULL);
	if (nod->last != NULL) r = nod->last;
	else r = nod->next;
	free(nod);
	endnod = r;
	return(r);
}

velocity(nod)
	LIST *nod;
{
	register float y, x, d, delta;
	if (nod == NULL) return;
	if (nod->last != NULL)
		{
		y = nod->fy - nod->last->fy;
		x = nod->fx - nod->last->fx;
		d = sqrt(y*y + x*x);
		if (d == 0.0) {
			nod->velocity = 0;
			return(0);
		}
		if ((delta = nod->time - nod->last->time) == 0.0)
			return(-1);
		nod->velocity = d / delta;
		}
	return(0);
}
