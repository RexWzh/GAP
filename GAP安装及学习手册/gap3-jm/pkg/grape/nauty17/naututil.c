/*****************************************************************************
*                                                                            *
* miscellaneous utilities for use with nauty 1.7.                            *
* None of these procedures are needed by nauty, but all are by dreadnaut.    *
*                                                                            *
*   Copyright (1984-1992) Brendan McKay.  All rights reserved.               *
*   Subject to waivers and disclaimers in nauty.h.                           *
*                                                                            *
*   CHANGE HISTORY                                                           *
*       10-Nov-87 : final changes for version 1.2                            *
*        5-Dec-87 : changes made for version 1.3 :                           *
*                   - added procedures readinteger() and readstring()        *
*                   - replaced all uses of fscanf() by appropriate uses      *
*                     of readinteger() or readstring()                       *
*                   - "N:" is now illegal in readgraph() if N is too large   *
*                     or too small                                           *
*       28-Sep-88 : renamed to version 1.4 (no changes to this file)         *
*       23-Mar-89 : changes for version 1.5 :                                *
*                   - declared key in hash()                                 *
*                   - changed file name to naututil.c                        *
*       29-Mar-89 : - declared workperm[] and workset[], and modified        *
*                     many routines to use them.                             *
*                   - added putmapping()                                     *
*                   - reworked some code in mathon() and rangraph()          *
*        3-Apr-89 : - changed putorbits() to use O(n) algorithm              *
*        5-Apr-89 : - modifed readgraph() to not require fresh line          *
*                   - changed MAKEEMPTY uses to EMPTYSET uses                *
*       26-Apr-89 : - moved ptncode() and equitable() to nautaux.c           *
*                   - added putquotient()                                    *
*       18-Aug-89 : - modified putset() to use "i:j" syntax optionally       *
*                   - modified putorbits() to use putset()                   *
*                   - modified calling sequence for mathon()                 *
*       19-Aug-90 : - changed delimeter arg of copycomment to int            *
*       14-Oct-90 : renamed to version 1.6 (no changes to this file)         *
*       23-Jan-91 : changes for version 1.7 :                                *
*                   - fixed bug in complement()                              *
*       27-Aug-92 : - made linelength <= 0 mean no line breaks
*                                                                            *
*****************************************************************************/

#define  EXTDEFS 1
#include "naututil.h"          /* which includes "nauty.h" and <stdio.h> */

#if  MAXM==1
#define M 1
#else
#define M m
#endif

long seed = 1;
static permutation workperm[MAXN+2];   /* used for scratch purposes */
static set workset[MAXM];              /* used for scratch purposes */

#ifdef  NLMAP
#define GETNW(c,f) do c = getc(f); while (c==' '||c=='\t')
#define GETNWC(c,f) do c = getc(f); while (c==' '||c==','||c=='\t')
#define GETNWL(c,f) do c = getc(f); while (c==' '||c=='\n'||c=='\t')
#else
#define GETNW(c,f) do c = getc(f); while (c==' '||c=='\t'||c=='\r')
#define GETNWC(c,f) do c = getc(f); while (c==' '||c==','||c=='\t'||c=='\r')
#define GETNWL(c,f) do c = getc(f); while (c==' '||c=='\n'||c=='\t'||c=='\r')
#endif

#define ISDIGIT(c) ((c) >= '0' && (c) <= '9')

/*****************************************************************************
*                                                                            *
*  setinter(set1,set2,m) = the number of elements in the intersection of     *
*  the sets set1 and set2.                                                   *
*                                                                            *
*****************************************************************************/

int
setinter(set1,set2,m)
register set *set1,*set2;
int m;
{
        register setword x;

#if  MAXM==1
        if (x = *set1 & *set2)
            return(POPCOUNT(x));
        else
            return(0);
#else
        register int count,i;

        count = 0;
        for (i = m; --i >= 0;)
            if (x = (*set1++) & (*set2++))     /* not == */
                count += POPCOUNT(x);

        return(count);
#endif
}

/*****************************************************************************
*                                                                            *
*  setsize(set1,m) = the number of elements in the set set1.                 *
*                                                                            *
*****************************************************************************/

int
setsize(set1,m)
register set *set1;
int m;
{

#if  MAXM==1
        if (set1 != 0)
            return(POPCOUNT(*set1));
        else
            return(0);
#else
        register int count,i;
        register setword x;

        count = 0;
        for (i = m; --i >= 0;)
            if (x = *set1++)             /* not == */
                count += POPCOUNT(x);

        return(count);
#endif
}

/*****************************************************************************
*                                                                            *
*  flushline(f) reads from f until '\n' or EOF.                              *
*  If non-trivial characters are skipped, the user is informed.              *
*                                                                            *
*****************************************************************************/

void
flushline(f)
FILE *f;
{
        boolean msg;
        int c;

        msg = FALSE;

        while ((c = getc(f)) != EOF && c != '\n')
            if (msg)
                PUTC((char)c,ERRFILE);
            else if (c != ' ' && c != '\t' && c != '\f' &&
                                      c != '\r' && c != ',')
            {
                msg = TRUE;
                fprintf(ERRFILE,"input skipped : '%c",(char)c);
            }
        if (msg)
            fprintf(ERRFILE,"'\n\n");
}

/*****************************************************************************
*                                                                            *
*  readinteger(f,&i) reads an optionally-signed integer from f, preceded by  *
*  any amount of white space.  The function value returned is TRUE if an     *
*  integer was found, FALSE otherwise.                                       *
*                                                                            *
*****************************************************************************/

boolean
readinteger(f,p)
FILE *f;
int *p;
{
        register int c,ans,minus;

        GETNWL(c,f);
        if (!ISDIGIT(c) && c != '-' && c != '+')
        {
            if (c != EOF)
                ungetc((char)c,f);
            return(FALSE);
        }

        minus = c == '-';
        ans = (c == '-' || c == '+' ? 0 : c - '0');

        c = getc(f);
        while (ISDIGIT(c))
        {
            ans *= 10;
            ans += c - '0';
            c = getc(f);
        }

        if (c != EOF)
            ungetc((char)c,f);

        *p = (minus ? -ans : ans);
        return(TRUE);
}

/*****************************************************************************
*                                                                            *
*  readstring(f,s) reads a non-white string from f, preceded by any amount   *
*  of white space.  The function value returned is TRUE if a non-white char  *
*  was found, FALSE otherwise.  The string itself goes into s terminated     *
*  by a null character.  A null string is in s if no string is found.        *
*                                                                            *
*****************************************************************************/

boolean
readstring(f,s)
FILE *f;
register char *s;
{
        register int c;

        GETNWL(c,f);
        if (c == EOF)
        {
            *s = '\0';
            return(FALSE);
        }

        *s++ = c;
        c = getc(f);
        while (c != ' ' && c != '\n' && c != '\t' && c != '\r' && c != EOF)
        {
            *s++ = c;
            c = getc(f);
        }
        *s = '\0';

        if (c != EOF)
            ungetc((char)c,f);
        return(TRUE);
}

/*****************************************************************************
*                                                                            *
*  getint(f) reads an integer from f, optionally preceded by '='             *
*  and white space.  -1 is returned if the attempt was unsuccessful.         *
*                                                                            *
*****************************************************************************/

int
getint(f)
FILE *f;
{
        int i,c;

        GETNWL(c,f);
        if (c != '=')
            ungetc((char)c,f);

        if (readinteger(f,&i))
            return(i);
        else
            return(-1);
}

/*****************************************************************************
*                                                                            *
*  putset(f,set1,curlenp,linelength,m,compress)   writes the set set1 to     *
*  file f using at most linelength characters per line (excluding '\n').     *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*  *curlenp is the number of characters on the line so far; it is updated.   *
*  A range j1,j1+1,...,j2 for j2-j1>=2 is written as "j1:j2" if compress     *
*  is nonzero (eg. TRUE); otherwise each element is written separately.      *
*  No final '\n' is written.  labelorg is used.                              *
*                                                                            *
*  FUNCTIONS CALLED: nextelement(),itos()                                    *
*                                                                            *
*****************************************************************************/

void
putset(f,set1,curlenp,linelength,m,compress)
FILE *f;
set *set1;
int linelength,m,*curlenp;
boolean compress;
{
        int slen,j1,j2;
        char s[40];

        j1 = -1;
        while ((j1 = nextelement(set1,m,j1)) >= 0)
        {
            j2 = j1;
            if (compress)
            {
                while (nextelement(set1,m,j2) == j2 + 1)
                    ++j2;
                if (j2 == j1+1)
                    j2 = j1;
            }
            slen = itos(j1 + labelorg,s);
            if (j2 >= j1 + 2)
            {
                s[slen] = ':';
                slen += 1 + itos(j2 + labelorg,&s[slen+1]);
            }

            if (linelength > 0 && *curlenp + slen + 1 >= linelength)
            {
                fprintf(f,"\n   ");
                *curlenp = 3;
            }
            fprintf(f," %s",s);
            *curlenp += slen + 1;
            j1 = j2;
        }
}

/*****************************************************************************
*                                                                            *
*  readgraph(f,g,digraph,prompt,edit,linelength,m,n) reads a graph g from f. *
*  Commands: (There is always a "current vertex" v, initially labelorg;      *
*             n is an unsigned integer.)                                     *
*  n  : add edge (v,n)                                                       *
*  -n : delete edge (v,n)                                                    *
*  n: : set v := n, and exit if v >= n.                                      *
*  ?  : type neighbours of vertex v                                          *
*  ;  : increment v, and exit if v >= n.                                     *
*  .  : exit                                                                 *
*  !  : skip rest of input line                                              *
*                                                                            *
* If digraph==FALSE, loops are illegal and (x,y) => (y,x)                    *
* If edit==FALSE, the graph is initialized to empty.                         *
* If prompt==TRUE, prompts are written to PROMPTFILE.                        *
* linelength is a limit on the number of characters per line caused by '?'   *
* A value of linelength <= 0 dictates no line breaks at all.                *
* labelorg is used.                                                          *
*                                                                            *
* FUNCTIONS CALLED : putset()                                                *
*                                                                            *
*****************************************************************************/

void
readgraph(f,g,digraph,prompt,edit,linelength,m,n)
FILE *f;
register graph *g;
boolean digraph,prompt,edit;
int linelength,m,n;
{
        register int v,c;
        int curlen,w;
        register graph *gv;
        boolean neg;

        if (!edit)
            for (v = 0, gv = g; v < n; ++v, gv += M)
                EMPTYSET(gv,m);

        v = 0;
        gv = g;
        neg = FALSE;

        while (TRUE)
        {
            GETNWC(c,f);
            if (ISDIGIT(c))
            {
                ungetc((char)c,f);
                readinteger(f,&w);
                w -= labelorg;
                if (neg)
                {
                    neg = FALSE;
                    if (w < 0 || w >= n || !digraph && w == v)
                        fprintf(ERRFILE,"illegal edge (%d,%d) ignored\n\n",
                                v + labelorg,w + labelorg);
                    else
                    {
                        DELELEMENT(gv,w);
                        if (!digraph)
                            DELELEMENT(GRAPHROW(g,w,M),v);
                    }
                }
                else
                {
                    GETNWC(c,f);
                    if (c == ':')
                        if (w < 0 || w >= n)
                            fprintf(ERRFILE,
                                    "illegal vertex number %d ignored\n\n",
                                    w + labelorg);
                        else
                        {
                            v = w;
                            gv = GRAPHROW(g,v,M);
                        }
                    else
                    {
                        ungetc((char)c,f);
                        if (w < 0 || w >= n || !digraph && w == v)
                            fprintf(ERRFILE,"illegal edge (%d,%d) ignored\n\n",
                                    v + labelorg,w + labelorg);
                        else
                        {
                            ADDELEMENT(gv,w);
                            if (!digraph)
                                ADDELEMENT(GRAPHROW(g,w,M),v);
                        }
                    }
                }
            }
            else switch(c)
            {
                case ';':
                    neg = FALSE;
                    ++v;
                    if (v >= n)
                        return;
                    gv = GRAPHROW(g,v,M);
                    break;
                case '?':
                    neg = FALSE;
                    fprintf(PROMPTFILE,"%2d : ",v + labelorg);
                    curlen = 5;
                    putset(PROMPTFILE,gv,&curlen,linelength,M,FALSE);
                    fprintf(PROMPTFILE,";\n");
                    break;
                case '\n':
                    neg = FALSE;
                    if (prompt)
                        fprintf(PROMPTFILE,"%2d : ",v + labelorg);
                    break;
                case EOF:
                case '.':
                    return;
                case '-':
                    neg = TRUE;
                    break;
                case '!':
                    do
                        c = getc(f);
                    while (c != '\n' && c != EOF);
                    if (c == '\n')
                        ungetc((char)c,f);
                    break;
                default :
                    fprintf(ERRFILE,"illegal char '%c' - use '.' to exit\n\n",
                            (char)c);
            }
        }
}

/*****************************************************************************
*                                                                            *
*  putgraph(f,g,linelength,m,n) writes a list of the edges of g to f         *
*  using at most linelength characters per line (excluding '\n').            *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*  labelorg is used.                                                         *
*                                                                            *
*  FUNCTIONS CALLED: putset()                                                *
*                                                                            *
*****************************************************************************/

void
putgraph(f,g,linelength,m,n)
FILE *f;
graph *g;
int linelength,m,n;
{
        int i,curlen;
        set *pg;

        for (i = 0, pg = g; i < n; ++i, pg += M)
        {
            fprintf(f,"%3d : ",i + labelorg);
            curlen = 7;
            putset(f,pg,&curlen,linelength,M,FALSE);
            fprintf(f,";\n");
        }
}

/*****************************************************************************
*                                                                            *
*  putmapping(f,lab1,org1,lab2,org2,linelength,n) writes n pairs             *
*  (lab1[i]+org1)-(lab2[i]+org2) to file f in increasing order of lab1[i].   *
*  lab1 and lab2 are assumed to be permutations.  At most linelength         *
*  characters (excluding '\n') are written per line.                         *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*                                                                            *
*  FUNCTIONS CALLED: itos(),putstring()                                      *
*                                                                            *
*****************************************************************************/

void
putmapping(f,lab1,org1,lab2,org2,linelength,n)
FILE *f;
nvector *lab1,*lab2;
int org1,org2,linelength,n;
{
        int i,curlen,slen;
        char s[60];

        for (i = 0; i < n; ++i)
            workperm[lab1[i]] = lab2[i];

        curlen = 0;
        for (i = 0; i < n; ++i)
        {
            slen = itos(i + org1,s);
            s[slen++] = '-';
            slen += itos(workperm[i]+org2,&s[slen]);
            if (linelength > 0 && curlen + slen + 1 > linelength)
            {
                putstring(f,"\n  ");
                curlen = 2;
            }
            PUTC(' ',f);
            putstring(f,s);
            curlen += slen + 1;
        }
        PUTC('\n',f);
}

/*****************************************************************************
*                                                                            *
*  putorbits(f,orbits,linelength,n) writes the orbits to f as lists          *
*  of integers separated by semicolons.  No more than linelength characters  *
*  (excluding '\n') are written per line.                                    *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*  labelorg is used.                                                         *
*                                                                            *
*  FUNCTIONS CALLED: itos(),putset()                                         *
*                                                                            *
*****************************************************************************/

void
putorbits(f,orbits,linelength,n)
FILE *f;
nvector *orbits;
int linelength,n;
{
        register int i,j;
        int m,curlen;

        m = (n + WORDSIZE - 1) / WORDSIZE;
        for (i = n; --i >= 0;)
            workperm[i] = 0;
        for (i = n; --i >= 0;)
            if ((j = orbits[i]) < i)
            {
                workperm[i] = workperm[j];
                workperm[j] = i;
            }

        curlen = 0;
        for (i = 0; i < n; ++i)
            if (orbits[i] == i)
            {
                EMPTYSET(workset,m);
                j = i;
                do
                {
                    ADDELEMENT(workset,j);
                    j = workperm[j];
                }
                while (j > 0);
                putset(f,workset,&curlen,linelength - 1,m,TRUE);
                PUTC(';',f);
                ++curlen;
            }
        PUTC('\n',f);
}

/*****************************************************************************
*                                                                            *
*  putquotient(f,g,lab,ptn,level,linelength,m,n) writes the quotient matrix  *
*  of g defined by the partition at the given level.  Precisely, for each    *
*  cell W, it writes the number w of the least vertex in W, then the size    *
*  of W, then the number of times w is joined FROM each cell.  A complete    *
*  join is written as "*", while an empty join is written as "-".  No more   *
*  than linelength  characters (excluding '\n') are written per line unless  *
*  linelength is very small.  A value of linelength <= 0 dictates no line    *
*  breaks at all.   labelorg is used.                                        *
*                                                                            *
*  FUNCTIONS CALLED: itos()                                                  *
*                                                                            *
*****************************************************************************/

void
putquotient(f,g,lab,ptn,level,linelength,m,n)
FILE *f;
graph *g;
nvector *lab,*ptn;
int level,linelength,m,n;
{
        register int i;
        char s[50];
        int ic,curlen,v,w,cell1,cell2,numcells,jc,csize,k;
        set *gw;

        numcells = 0;
        for (cell1 = 0; cell1 < n; cell1 = cell2 + 1)
        {
            for (cell2 = cell1; ptn[cell2] > level; ++cell2)
                {}
            w = lab[cell1];
            for (i = cell1 + 1; i <= cell2; ++i)
                if (lab[i] < w)
                    w = lab[i];
            workperm[numcells++] = w;
        }

        for (ic = cell1 = 0; ic < numcells; ++ic, cell1 = cell2 + 1)
        {
            for (cell2 = cell1; ptn[cell2] > level; ++cell2)
                {}
            EMPTYSET(workset,M);
            for (i = cell1; i <= cell2; ++i)
                ADDELEMENT(workset,lab[i]);
            v = workperm[ic];
            csize = cell2 - cell1 + 1;
            if (v + labelorg < 10)
            {
                s[0] = ' ';
                curlen = 1;
            }
            else
                curlen = 0;
            curlen += itos(v + labelorg,&s[curlen]);
            s[curlen++] = '[';
            curlen += itos(csize,&s[curlen]);
            fprintf(f,"%s",s);
            if (csize < 10)
            {
                fprintf(f,"]  :");
                curlen += 4;
            }
            else
            {
                fprintf(f,"] :");
                curlen += 3;
            }

            for (jc = 0; jc < numcells; ++jc)
            {
                w = workperm[jc];
                gw = GRAPHROW(g,w,m);
                k = setinter(gw,workset,M);
                if (k == 0 || k == csize)
                {
                    if (linelength > 0 && curlen + 2 > linelength)
                    {
                        fprintf(f,"\n    ");
                        curlen = 4;
                    }
                    if (k == 0)
                        fprintf(f," -");
                    else
                        fprintf(f," *");
                    curlen += 2;
                }
                else
                {
                    i = itos(k,s);
                    if (linelength > 0 && curlen + i + 1 > linelength)
                    {
                        fprintf(f,"\n    ");
                        curlen = 4;
                    }
                    fprintf(f," %s",s);
                    curlen += i + 1;
                }
            }
            fprintf(f,"\n");
        }
}

/*****************************************************************************
*                                                                            *
*  putptn(f,lab,ptn,level,linelength,n) writes the partition at the given    *
*  level as sorted lists of integers separated by semicolons.  No more than  *
*  linelength characters (excluding '\n') are written per line.              *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*  labelorg is used.                                                         *
*                                                                            *
*  FUNCTIONS CALLED: itos(),putset()                                         *
*                                                                            *
*****************************************************************************/

void
putptn(f,lab,ptn,level,linelength,n)
FILE *f;
nvector *lab,*ptn;
int level,linelength,n;
{
        register int i;
        int curlen,m;

        m = (n + WORDSIZE - 1) / WORDSIZE;
        PUTC('[',f);
        curlen = 1;
        i = 0;
        while (i < n)
        {
            EMPTYSET(workset,m);
            while (TRUE)
            {
                ADDELEMENT(workset,lab[i]);
                if (ptn[i] > level)
                    ++i;
                else
                    break;
            }
            putset(f,workset,&curlen,linelength - 2,m,TRUE);
            if (i < n-1)
            {
                fprintf(f," |");
                curlen += 2;
            }
            ++i;
        }
        fprintf(f," ]\n");
}

/*****************************************************************************
*                                                                            *
*  putcanon(f,canonlab,canong,linelength,m,n) writes the label canonlab      *
*  and the graph canong to f, using at most linelength characters            *
*  (excluding '\n') per line.   labelorg is used.                            *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*                                                                            *
*  FUNCTIONS CALLED: writeperm(),putgraph()                                  *
*                                                                            *
*****************************************************************************/

void
putcanon(f,canonlab,canong,linelength,m,n)
FILE *f;
nvector *canonlab;
graph *canong;
int linelength,m,n;
{
        register int i;

        for (i = 0; i < n; ++i)
            workperm[i] = canonlab[i];
        writeperm(f,workperm,TRUE,linelength,n);
        putgraph(f,canong,linelength,m,n);
}

/*****************************************************************************
*                                                                            *
*  readptn(f,lab,ptn,&numcells,prompt,n) reads a partition from f            *
*  and establishes it in (lab,ptn).                                          *
*  The format can be just a number, which is fixed alone, or an arbitrary    *
*  partition [...|...|...].  Ranges x:y can be used.                         *
*  labelorg is used.                                                         *
*                                                                            *
*****************************************************************************/

void
readptn(f,lab,ptn,numcells,prompt,n)
FILE *f;
nvector *lab,*ptn;
int *numcells,n;
boolean prompt;
{
        register int i,j;
        int c,v1,v2,m;

        m = (n + WORDSIZE - 1) / WORDSIZE;
        GETNW(c,f);
        if (c == '=')
            GETNW(c,f);
        if (ISDIGIT(c))
        {
            ungetc((char)c,f);
            readinteger(f,&v1);
            v1 -= labelorg;
            if (v1 >= 0 && v1 < n)
                fixit(lab,ptn,numcells,v1,n);
            else
            {
                fprintf(ERRFILE,"vertex out of range (%d), fixing nothing\n\n",
                        v1 + labelorg);
                unitptn(lab,ptn,numcells,n);
            }
            return;
        }
        if (c != '[')
        {
            ungetc((char)c,f);
            fprintf(ERRFILE,"illegal partition, fixing nothing\n\n");
            unitptn(lab,ptn,numcells,n);
            return;
        }
        EMPTYSET(workset,m);
        *numcells = 0;
        for (i = 0; i < n; ++i)
            ptn[i] = INFINITY;
        i = 0;
        j = -1;
        while (TRUE)
        {
            GETNWC(c,f);
            if (ISDIGIT(c))
            {
                ungetc((char)c,f);
                readinteger(f,&v1);
                v1 -= labelorg;
                GETNWC(c,f);
                if (c == ':')
                    if (!readinteger(f,&v2))
                    {
                        fprintf(ERRFILE,"unfinished range\n\n");
                        v2 = v1;
                    }
                    else
                        v2 -= labelorg;
                else
                {
                    ungetc((char)c,f);
                    v2 = v1;
                }
                while (v1 <= v2)
                {
                    if (v1 < 0 || v1 >= n || ISELEMENT(workset,v1))
                        fprintf(ERRFILE,"illegal or repeated number : %d\n\n",
                                v1 + labelorg);
                    else
                    {
                        ADDELEMENT(workset,v1);
                        lab[++j] = v1;
                    }
                    ++v1;
                }
            }
            else if (c == '|' || c == ']' || c == EOF)
            {
                if (j >= i)
                {
                    ++*numcells;
                    ptn[j] = 0;
                }
                if (c == '|')
                    i = j + 1;
                else if (j == n - 1)
                    return;
                else
                {
                    i = j + 1;
                    ++*numcells;
                    for (j = 0; j < n; ++j)
                        if (!ISELEMENT(workset,j))
                            lab[i++] = j;
                    ptn[n-1] = 0;
                    return;
                }
            }
            else if (c == '\n')
            {
                if (prompt)
                    fprintf(PROMPTFILE,"] ");
            }
            else
                fprintf(ERRFILE,"illegal character '%c' in partition\n\n",c);
        }
}

/*****************************************************************************
*                                                                            *
*  unitptn(lab,ptn,&numcells,n) establishes the partition with one cell.     *
*                                                                            *
*****************************************************************************/

void
unitptn(lab,ptn,numcells,n)
nvector *lab,*ptn;
int *numcells,n;
{
        register int i;

        for (i = 0; i < n; ++i)
        {
            lab[i] = i;
            ptn[i] = INFINITY;
        }
        ptn[n-1] = 0;
        *numcells = 1;
}

/*****************************************************************************
*                                                                            *
*  cellstarts(ptn,level,cell,m,n) sets the set cell to contain the indices   *
*  of the starts in ptn of the partition at level level.                     *
*                                                                            *
*****************************************************************************/

void
cellstarts(ptn,level,cell,m,n)
nvector *ptn;
int level,m,n;
set *cell;
{
        register int i;

        EMPTYSET(cell,m);
        i = 0;
        while (i < n)
        {
            ADDELEMENT(cell,i);
            while (ptn[i] > level)
                ++i;
            ++i;
        }
}

/*****************************************************************************
*                                                                            *
*  fixit(lab,ptn,&numcells,fixedvertex,n) establishes the partition          *
*  with one cell {fixedvertex} and all the other vertices (if any) in        *
*  another cell.                                                             *
*                                                                            *
*****************************************************************************/

void
fixit(lab,ptn,numcells,fixedvertex,n)
nvector *lab,*ptn;
int *numcells,fixedvertex,n;
{
        register int i;

        for (i = 1; i < n; ++i)
        {
            lab[i] = i;
            ptn[i] = 1;
        }

        lab[0] = fixedvertex;
        lab[fixedvertex] = 0;
        ptn[0] = 0;
        ptn[n-1] = 0;
        if (n == 1)
            *numcells = 1;
        else
            *numcells = 2;
}

/*****************************************************************************
*                                                                            *
*  hash(setarray,length,key) is a function whose value depends only on the   *
*  first 'length' entries of the array 'setarray', and the value of key.     *
*  key should be in the range 1..31 and preferably odd.                      *
*  This works best if long ints have at least 32 bits, but it's valid anyway.*
*                                                                            *
*****************************************************************************/

long hash(setarray,length,key)
register set *setarray;
long length;
int key;
{
        register long code;
        register set *sptr;

        code = length;
        sptr = setarray + length;

        while (--sptr >= setarray)
            code = (code<<key) ^ (code>>(32-key)) + *sptr;

        return(code);
}

/*****************************************************************************
*                                                                            *
*  readperm(f,perm,prompt,n) reads a permutation of order n from             *
*  f, terminated by a semicolon.  Any repeated or illegal numbers or         *
*  characters are reported then ignored.    Missing numbers are filled in    *
*  in numerical order.  A prompt is issued for each line if prompt!=FALSE.   *
*  labelorg is used.                                                         *
*                                                                            *
*****************************************************************************/

void
readperm(f,perm,prompt,n)
FILE *f;
permutation *perm;
int n;
boolean prompt;
{
        register int i;
        int m,c,j;

        m = (n + WORDSIZE - 1) / WORDSIZE;
        EMPTYSET(workset,m);

        i = 0;

        while (TRUE)
        {
            GETNWC(c,f);
            if (c == ';' || c == EOF)
                break;
            if (ISDIGIT(c))
            {
                ungetc((char)c,f);
                readinteger(f,&j);
                j -= labelorg;
                if (j >= 0 && j < n && i < n && !ISELEMENT(workset,j))
                {
                    perm[i++] = j;
                    ADDELEMENT(workset,j);
                }
                else
                    fprintf(ERRFILE,
                          "illegal or repeated number in permutation : %d\n\n",
                          j + labelorg);
            }
            else
            {
                if (c == '\n' && prompt)
                    fprintf(PROMPTFILE,"+ ");
                if (c != '\n')
                    fprintf(ERRFILE,"bad character '%c' in permutation\n\n",
                           (char)c);
            }
        }

        for (j = 0; j < n; ++j)
            if (!ISELEMENT(workset,j))
                perm[i++] = j;
}

/*****************************************************************************
*                                                                            *
*  ranperm(perm,n) creates a random permutation in perm.                     *
*                                                                            *
*****************************************************************************/

void
ranperm(perm,n)
permutation *perm;
int n;
{
        int i,j,t;

        for (i = n; --i >= 0; )
            perm[i] = i;

        for (i = n; --i > 0; )
        {
            j = RAN(i+1);
            t = perm[i];
            perm[i] = perm[j];
            perm[j] = t;
        }
}

/*****************************************************************************
*                                                                            *
*  relabel(g,perm,lab,workg,m,n) replaces g by g^perm, using workg as        *
*  scratch space.  If lab!=NULL, it is taken as a labelling vector and       *
*  also permuted.                                                            *
*                                                                            *
*  FUNCTIONS CALLED: updatecan()                                             *
*                                                                            *
*****************************************************************************/

void
relabel(g,lab,perm,workg,m,n)
graph *g,*workg;
nvector *lab;
permutation *perm;
int m,n;
{
        register long li;
        register int i;

        for (li = (long)M * (long)n; --li >= 0;)
            workg[li] = g[li];

        updatecan(workg,g,perm,0,M,n);
        if (lab != (nvector*)NULL)
        {
            for (i = 0; i < n; ++i)
                workperm[perm[i]] = i;
            for (i = 0; i < n; ++i)
                lab[i] = workperm[lab[i]];
        }
}

/*****************************************************************************
*                                                                            *
*  copycomment(fin,fout,delimiter) copies fin to fout until either EOF or    *
*  the character 'delimiter' is read.  The delimiter itself isn't written.   *
*  Escape sequences \n,\t,\b,\r,\f,\\,\',\",\\n are recognised.  Otherwise,  *
*  '\' is ignored.                                                           *
*                                                                            *
*****************************************************************************/

void
copycomment(fin,fout,delimiter)
FILE *fin,*fout;
int delimiter;
{
        register int c,backslash;

        backslash = FALSE;

        while ((c = getc(fin)) != EOF && (c != delimiter || backslash))
            if (backslash)
            {
                switch (c)
                {
                case '\n':
                    break;
                case 'n':
                    PUTC('\n',fout);
                    break;
                case 't':
                    PUTC('\t',fout);
                    break;
                case 'b':
                    PUTC('\b',fout);
                    break;
                case 'r':
                    PUTC('\r',fout);
                    break;
                case 'f':
                    PUTC('\f',fout);
                    break;
                case '\\':
                    PUTC('\\',fout);
                    break;
                case '\'':
                    PUTC('\'',fout);
                    break;
                case '"':
                    PUTC('"',fout);
                    break;
                default:
                    PUTC(c,fout);
                }
                backslash = FALSE;
            }
            else if (c == '\\')
                backslash = TRUE;
            else
                PUTC(c,fout);
}

/*****************************************************************************
*                                                                            *
*  mathon(g1,m1,n1,g2,m2,n2) performs a Mathon doubling operation on g1,     *
*  leaving the result in g2.                                                 *
*  m1,n1 and m2,n2 are the values of m,n before and after the operation.     *
*                                                                            *
*****************************************************************************/

void
mathon(g1,m1,n1,g2,m2,n2)
graph *g1,*g2;
int m1,n1,m2,n2;
{
        register int i,j,ii,jj;
        long li;
        register set *rowptr,*gp;

        for (li = (long)m2 * (long)n2; --li >= 0;)
            g2[li] = 0;

        for (i = 1; i <= n1; ++i)
        {
            ii = i + n1 + 1;
            gp = GRAPHROW(g2,0,m2);        /* unnecessarily convoluted code */
            ADDELEMENT(gp,i);             /* needed to avoid compiler bug  */
            gp = GRAPHROW(g2,i,m2);        /* in MSDOS version */
            ADDELEMENT(gp,0);
            gp = GRAPHROW(g2,n1+1,m2);
            ADDELEMENT(gp,ii);
            gp = GRAPHROW(g2,ii,m2);
            ADDELEMENT(gp,n1+1);
        }

        for (i = 0, rowptr = g1; i < n1; ++i, rowptr += m1)
            for (j = 0; j < n1; ++j)
                if (j != i)
                {
                    ii = i + n1 + 2;
                    jj = j + n1 + 2;
                    if (ISELEMENT(rowptr,j))
                    {
                        gp = GRAPHROW(g2,i+1,m2);
                        ADDELEMENT(gp,j+1);
                        gp = GRAPHROW(g2,ii,m2);
                        ADDELEMENT(gp,jj);
                    }
                    else
                    {
                        gp = GRAPHROW(g2,i+1,m2);
                        ADDELEMENT(gp,jj);
                        gp = GRAPHROW(g2,ii,m2);
                        ADDELEMENT(gp,j+1);
                    }
                }
}

/*****************************************************************************
*                                                                            *
*  rangraph(g,digraph,invprob,m,n) makes a random graph (or digraph if       *
*  digraph!=FALSE) with edge probability 1/invprob.                          *
*                                                                            *
*****************************************************************************/

void
rangraph(g,digraph,invprob,m,n)
register graph *g;
boolean digraph;
int invprob,m,n;
{
        register int i,j;
        long li;
        set *row,*col;

        for (li = (long)m * (long)n; --li >= 0;)
                g[li] = 0;

        for (i = 0, row = g; i < n; ++i, row += m)
            if (digraph)
            {
                for (j = 0; j < n; ++j)
                    if (RAN(invprob) == 0)
                        ADDELEMENT(row,j);
            }
            else
                for (j = i + 1, col = GRAPHROW(g,j,m); j < n; ++j, col += m)
                    if (RAN(invprob) == 0)
                    {
                        ADDELEMENT(row,j);
                        ADDELEMENT(col,i);
                    }
}

/*****************************************************************************
*                                                                            *
*  putdegs(f,g,linelength,m,n)  writes the degree of each vertex of g to     *
*  file f, using at most linelength characters per line (excluding '\n').    *
*  A value of linelength <= 0 dictates no line breaks at all.                *
*  labelorg is used.                                                         *
*                                                                            *
*  FUNCTIONS CALLED : itos(),putstring(),setsize()                           *
*                                                                            *
*****************************************************************************/

void
putdegs(f,g,linelength,m,n)
FILE *f;
graph *g;
int linelength,m,n;
{
        char s[60];
        int i,j,v1,v2,deg,curlen;
        graph *gp;

        for (i = 0, gp = g; i < n; ++i, gp += M)
            workperm[i] = setsize(gp,m);

        curlen = 0;
        v1 = 0;
        while (v1 < n)
        {
            deg = workperm[v1];
            v2 = v1;
            for (v2 = v1; v2 < n - 1 && workperm[v2+1] == deg; ++v2)
            {}
            j = itos(v1 + labelorg,s);
            if (v2 > v1)
            {
                s[j++] = '-';
                j += itos(v2 + labelorg,&s[j]);
            }
            s[j++] = ':';
            j += itos(deg,&s[j]);
            s[j] = ' ';
            s[j+1] = '\0';
            if (linelength > 0 && curlen + j >= linelength)
            {
                PUTC('\n',f);
                curlen = 0;
            }
            curlen += j + 1;
            putstring(f,s);
            v1 = v2 + 1;
        }
        PUTC('\n',f);
}

/*****************************************************************************
*                                                                            *
*  complement(g,m,n) replaces the graph g by its complement                  *
*  No loops are created unless there are loops present, in which case the    *
*  loops are also complemented.                                              *
*                                                                            *
*****************************************************************************/

void
complement(g,m,n)
register graph *g;
int m,n;
{
        boolean loops;
        register int i,j;
        register graph *gp;

        loops = FALSE;
        for (i = 0, gp = g; i < n && !loops; ++i, gp += M)
            if (ISELEMENT(gp,i))
                loops = TRUE;

        EMPTYSET(workset,m);
        for (i = 0; i < n; ++ i)
            ADDELEMENT(workset,i);

        for (i = 0, gp = g; i < n; ++i, gp += M)
        {
            for (j = 0; j < M; ++j)
                gp[j] = workset[j] & ~gp[j];
            if (!loops)
                DELELEMENT(gp,i);
        }
}
