/* UNIT GENERATOR GLOBALS */
/*
   To add a unit generator to Cmusic, add its function call to the
   table of calls which follows this comment.  Then add an entry to the
   ug_vec structure array consisting of the triple {a, b, c}, where a
   is a string defining the name by which the unit generator is to be
   known in the score, b is a string describing the function of each
   argument, and c is the call to the unit generator program itself.
   Each argument is described by giving an arbitrary name to its
   function (such as "in" or "out"), followed by a list enclosed
   in square brakets which indicates the allowable argument types (b =
   io block, v = static variable, d = dynamic variable, p = note
   parameter, f = function, s = a string, n = number (e.g., a
   constant), and c = a program call). An asterisk after the right
   bracket indicates that any number of such arguments may be given;
   this may occur only on the last argument.
*/
/* Table of Contents (Lexicographical) */
int
    abs(), adn(), airabsorb(),
    band(), blp(),
    delay(), diff(), _div(),
    expon(),
    flt(), fltdelay(), fosc(), freq(),
    illus(), integ(), integer(), inv(), iosc(),
    janus(),
    logar(), lookup(),
    map(), mult(),
    neg(), nres(),
    osc(), out(),
    quant(),
    rah(), ran(),
    sah(), seg(), shape(), show(), showpath(), signum(), smooth(), sndfile(),
	space(), splice(), square(), sqroot(),
    test(), transit(),
    version(),
    white(),
    zdelay()
;

struct ug_desc ug_vec[] = {
    { "abs",
      "out[b], in[bnpv]",
       abs
    },
    { "adn",
      "out[b], in[bnpv]*",
       adn
    },
    { "airabsorb",
      "out[b], in[b], x[bnpv], y[bnpv], lx[pvd], ly[pvd], r[pvd], scale[pvd], c1[pvd], c2[pvd], xm1[pvd], xm2[pvd], ym1[pvd], ym2[pvd]",
      airabsorb
    },
    { "band",
"out[b], amp[bnpv], afac[bnpv], inca[bnpv], incb[bnpv], ifac[bnpv], table[fnpv], sum[dpv]",
       band
    },
    { "blp",
"out[b], amp[bnpv], inc1[bnpv], inc2[bnpv], n[bnpv], sum1[dpv], sum1[dpv]",
       blp
    },
    { "delay",
      "out[b], in[bnpv], gain[bnpv], table[fnpv], len[npv], pos[npv]",
       delay
    },
    { "diff",
      "out[b], in[bnpv], temp[dpv]",
       diff
    },
    { "_div",
      "out[b], numer[bnpv], denom[bnpv], temp[dpv]",
       _div
    },
    { "expon",
      "out[b], in[bnpv]",
       expon
    },
    { "flt",
      "out[b], in[bnpv], gain[bnpv], a0[bnpv], a1[bnpv], a2[bnpv], b1[bnpv], b2[bnpv], t1[dpv], t2[dpv], t3[dpv], t4[dpv]",
       flt
    },
    { "fltdelay",
      "out[b], dbuf[dpnv], dlen[dpnv], now[dpnv], del[dpnv], coef[dpnv], durcnt[dpnv], noicnt[dpnv], begcnt[dpnv], shrink[dpnv], strtch[dpnv], sdelay[dpnv], oldpit[dpnv], oldval[dpnv], oldin[dpnv], oldout[dpnv], in[bnpv], pitch[bnpv], decay[bnpv], table[fnpv], level[npv], final[npv], onset[npv], place[npv], filtr[npv], noise[npv], stiff[npv]",
       fltdelay
    },
    { "freq",
      "out[b], incr[bnpv], table[fnpv], sum[bdnpv]",
       freq
    },
    { "illus",
      "ampout[b], incrout[b], ampin[bnpv], incrin[bnpv], which[bnpv], ratio[bnpv], table[fnpv], incrmin[bnpv], incrmax[bnpv]",
       illus
    },
    { "integ",
      "out[b], in[bnpv], limit[bnpv], sum[dpv]",
       integ
    },
    { "integer",
      "out[b], in[bnpv]",
       integer
    },
    { "inv",
      "out[b], in[bnpv]",
       inv
    },
    { "iosc",
      "out[b], amp[bnpv], incr[bnpv], table[fnpv], sum[bdnpv]",
       iosc
    },
    { "janus",
      "out[b], in[bnpv], front[npv], back[npv], where[d]",
       janus
    },
    { "mult",
      "out[b], in[bnpv]*",
       mult
    },
    { "neg",
      "out[b], in[bnpv]",
       neg
    },
    { "nres",
      "out[b], in[bnpv], gain[bnpv], cf[bnpv], bw[bnpv], t1[dpv], t2[dpv], t3[dpv], t4[dpv], t5[dpv], t6[dpv], t7[dpv], t8[dpv], t9[dpv], t10[dpv]",
       nres
    },
    { "logar",
      "out[b], in[bnpv]",
       logar
    },
    { "lookup",
      "out[b], table[fnpv], in[bnpv], min[npv], max[npv]",
       lookup
    },
    { "map",
      "out[b], in[bnpv], imin[bnpv], imax[bnpv], omin[bnpv], omax[bnpv], clip[npv]",
       map
    },
    { "osc",
      "out[b], amp[bnpv], incr[bnpv], table[fnpv], sum[bdnpv]",
       osc
    },
    { "out",
      "in[bnpv]*",
       out
    },
    { "quant",
      "out[b], in[bnpv], imin[bnpv], imax[bnpv], steps[bnpv]",
       quant
    },
    { "rah",
      "out[b], amp[bnpv], incr[bnpv], pos[dpv], from[dpv], to[dpv]",
       rah
    },
    { "ran",
      "out[b], amp[bnpv], incr[bnpv], pos[dpv], from[dpv], to[dpv]",
       ran
    },
    { "sah",
      "out[b], in[b], period[bnpv], temp1[dnpv], temp2[dnpv]",
       sah
    },
    { "seg",
      "out[b], amp[bnpv], table[fnpv], sum[dpv], incr[npv]*",
       seg
    },
    { "shape",
      "out[b], table[fnpv], sum[bdnpv]",
       shape
    },
    { "show",
      "in[bnpv]*",
       show
    },
    { "showpath",
      "x[bnpv], y[bnpv], filename[s], temp1[pvd], temp2[pvd], temp3[pvd]",
       showpath
    },
    { "signum",
      "out[b], in[bnpv]",
       signum
    },
    { "smooth",
      "out[b], in[bnpv], tau[bnpv], temp[pvd]",
       smooth
    },
    { "sndfile",
      "out[b], amp[bnpv], incr[bnpv], filename[s], channel[npv], startframe[bnpv], endframe[bnpv], pos[bdnpv], ptr[pvd]",
       sndfile
    },
    { "space",
      "in[bnpv], nskip[npdv], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], t[pvd], x[bnpv], y[bnpv], theta[bnpv], amp[bnpv], back[bnpv]*",
       space
    },
    { "splice",
      "out[b], amp[bnpv], incr[bnpv], filename[s], channel[npv], startframe[bnpv], endframe[bnpv], pos[bdnpv], ptr[pvd], old[dnpv], older[dnpv], oldest[dnpv]",
      splice
    },
    { "square",
      "out[b], in[bnpv]",
       square
    },
    { "sqroot",
      "out[b], in[bnpv]",
       sqroot
    },
    { "test",
      "condition[npv], action[npv], in[bnpv]*",
       test
    },
    { "trans",
"out[b],no[dpv],len[dpv],i[dpv],t[npv],v[npv],a[npv]*,t[npv],v[npv]",
       transit
    },
    { "version",
      "out[b], amp[bnpv], incr[bnpv], filename[s], channel[npv], a[npv], b[npv], c[npv], d[npv], tau[npv], Rorig[npv], Forig[npv], ipos[dnpv], fpos[dnpv], lasto[dnpv], ptr[dnpv], sum[dnpv]",
       version
    },
    { "white",
      "out[b], amp[bnpv]",
       white
    },
    { "zdelay",
      "out[b], in[bnpv], maxtime[npv], table[dnpv], tlen[dnpv], pos[dnpv], gain[bnpv], delay[bnpv]*",
       zdelay
    },
    {0,0,0} /* LAST ENTRY IN UG_VEC */
};
