# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# define PSP   250000
# define SPACE 800000
# define SVSP   50000
# define  NPT    32767
# define  MP     500
# define  MB      80
# define  MREL    256
# define DNWDS    MREL/32
/* SPACE is space available for coset enumeration.
   MREL is max no of relations allowed.
   We will be using a boolean array done on the relator set, so DNWDS is the
   max no. of 32 bit words we need for the relator set.
*/

extern int grprog();
char inf[80],outf[80],outfg[80],firstnew,gap;
/* Default inf=gpname.sg
   outf=inf.rel anyway.
*/
int  perm[PSP],sv[SVSP],cp[5*NPT],orb[NPT+1],imsp[SPACE],*done[DNWDS],
       base[MB],lorb[MB],pno[MP/2],*pptr[MP],*svptr[MB],*imcos[MP],
       inv[MP],rno[MB],mp=MP,mrel=MREL,
       dnwds=DNWDS,mpt=NPT,mb=MB;
int    psp=PSP,space=SPACE,svsp=SVSP;

void main(argc,argv)  int argc;  char *argv[];
{ short arg;  char c,err;
  err=0; firstnew=0; arg=1; gap=0;
  if (argc<=arg) {err=1; goto error;}
  while (argv[arg][0]=='-')
  { c=argv[arg][1];
    if (c=='g') gap=1;
    else if (c=='f') firstnew=1;
    else {err=1; goto error;}
    arg++; if (argc<=arg) {err=1; goto error;}
  }
  strcpy(inf,argv[arg]); strcat(inf,".");
  if (gap) {strcpy(outfg,inf); strcat(outfg,"relg");}
  arg++; if (argc<=arg) strcat(inf,"sg"); else strcat(inf,argv[arg]);
  strcpy(outf,inf); strcat(outf,".rel");
  if (grprog()== -1) exit(1);
  error:  if (err)
  { fprintf(stderr,"Usage:   grrun [-f] [-g] gpname [inf]\n");
    exit(1);
  }
  exit(0);
}
