int addsv(short pt, short *sv);
int action(short *a,short *b);
int reduce(short *a);
int comp(short *a,short **dp);
int concheck();
int crprog1();
int crprog2();
int invwd(short *a,short *b);
int concat(short *a,short *b);
int compb(short *a,short **dp);
int ainvb(short *a,short *b,short *c);
int gc();
int allorbs(short *lorb, short *orno);
int assoc(short g1,short g2,short g3);
int backimage(short pt);
int cbdef(short gb,short ge,short cbno,short *d1,short *d2,short *wt,short *acl);
int cnprg1(void);
int cnprg2(void);
int collect(short *spc,short *spf,short sgn);
int comm(short *v,short *w,short **a);
int comprels();
int conprog(char con);
int egprog();
int expimage(short pt);
int exprep(short pt, short no, short *sv);
int express(short *p,short *relc,short nwt);
int extpprog();
int gpprog();
int image (short pt);
int intact();
int intbase(short pt,short pos,short *stad,short *nbad,short *b,short *lorb,short **svptr);
int intgen(short i,short j);
int inv(short **a,short **b);
int mcprog();
int nprg1();
int nprg2();
int nqmprog();
int nqprog();
int orbitsv(short pt, short *sv, short lo);
int pcprog();
int printbaselo(short nb, short *base, short *lorb);
int printpsv(short nb, short *gno, short **svptr);
int printvec (short *ptr, short e);
int readbaselo(short nb, short *base, short *lorb);
int readperm(short *ptr);
int readpsv(short e, short nb, short nperms, short **svptr);
int readvec(short *ptr, short e);
int scanrel();
int scprog();
int setpinv();
int spact();
int subrel(short i,short j);
int sylprog();
void addsvb (int pt, int **sv);
void addsvf (int pt, int **sv);
void advance(void);
void bgc();
void firstgen(short *p,short *hg,short *co);
void invert(short *a, short *b);
void printmat(short **a);
void prod(short *cm,short **a);
void readmat(short **a);
void seeknln(void);
void trans(short **a,short **b);
void zero(short *p1,short *p2);
