#define AUSWDEV "/dev/ct1"
#define OUT(x) (x & 077)
#define IN(x) ((x & 077) << 6)
#define MKBRK(x) (((x=='+')?1:0)<<12)
#define STRB (1 << 13)
#define INIT (1 << 15)
