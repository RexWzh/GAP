/* File miscio.c - 11. 10. 94.
 * 13. 3. 95. - skip_gap_expression written
 * 27. 2. 95. - allowed generators of form string^-1 - read_ident gets
 *              an extra parameter.
 * 12. 12. 94. transferred the function process_names to this file.
 * This file contains some miscellaneous input and output functions that
 * are used by both automata and string-rewriting programs.
 */

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "fsa.h"
#include "externals.h"

/* Functions defined in this file: */
boolean isdelim();
boolean isvalid();
int read_char();
void read_delim();
void read_token();
void skip_gap_expression();
void read_ident();
void read_int();
void read_string();
void process_names();
void read_word();
void printbuffer();
void add_to_buffer();
int add_word_to_buffer();
int add_iword_to_buffer();
int int_len();
boolean is_int();

boolean
isdelim(c)
        int c;
/* Note: '.' is not included in list of delimiters, since identifiers may
 * contain dots.
 * Integers are exceptional, however, in that they may be terminated by a
 * '.' as well as by as delimiter.
 * identifiers, strings and words are terminated by a delimiter.
 */
{ return c=='^' || c=='*' || c=='[' || c==']' || c==',' || c=='(' ||
         c==')' || c==';' || c=='\"' || c== ':' || c==EOF;
}

boolean
isvalid(c)
        int c;
/* Checks if symbol is valid in an identifier */
{ return isalpha(c) || isdigit(c) || c=='_' || c=='.';
}

int
read_char(rfile)
        FILE * rfile;
/* This is the basic character-reading function, which all other routines
 * call.
 * It skips over a backslash '\' followed by a newline '\n'.
 * It replaces tabs, newlines and comments (starting with # ending with
 * carriage return) with a space.
 * Otherwise it returns the next character read.
 * Use getc diretcly when readin literal strings.
 */
{ static int last; /* used to remember a character from the last call */
  int n;
  n = last ? last : getc(rfile);
  /* The input may contain a '\' character which is printed before '\n'
   * -- this is particularly true if the input file is generated by GAP
   */
  while (n=='\\') {
     n = getc(rfile);
     if (n == '\n')
       n = getc (rfile);
     else {
       last=n;
       return (int) '\\';
     }
  }
  if (n=='#')
    while ((n=getc(rfile))!='\n' && n!=EOF);
  if (n=='\n' || n=='\t')
    return(' ');
  return(n);
}

void
check_next_char(rfile, c)
        FILE * rfile;
        int c;
/* The next non-white character is expected to be 'c'.
   If not exit with error message.
*/
{ int n;
  while ((n=read_char(rfile))==' ');
  if (n!=c){
    fprintf(stderr,"#Input error: \'%c\' expected (not %c).\n",c,n);
    exit(1);
  }
}

void
read_delim(rfile,delim)
        FILE * rfile;
        int * delim;
/* Read next delimiter. */
{ int n;
  while ((n=read_char(rfile))==' ');
  if (!isdelim(n)){
    fprintf(stderr,"#Input error: delimiter expected (not %c).\n",n);
    exit(1);
  }
  *delim = n;
}

void
read_token(rfile,token,delim)
        FILE * rfile;
	char * token;
        int * delim;
/* Read stuff into token up to next delimiter or space  -
 * leading spaces are removed from stuff.
 */
{ int n;
  *token = '\0';
  while ((n=read_char(rfile))==' ');
  while (!isdelim(n) && !(n==' ')) {
     *(token++) = n; n=read_char(rfile);
  }
  *token = '\0';
  *delim = n;
}

void
skip_gap_expression(rfile,delim)
        FILE * rfile;
        int * delim;
/* Skip the next GAP expression -
 * probably not perfect, but should be adequate for the use intended.
 */
{ int n, br_ct=0;
  char br_rec[1024];

  while (1) {
     read_token(rfile,buffer,delim);
     if (strcmp(buffer,"function")==0)
        br_rec[br_ct++] = 'f'; /*marks that we are in a function definition */
     else if (strcmp(buffer,"end")==0) {
        if (br_ct>0 && br_rec[br_ct-1]=='f')
           br_ct--;
        else{
          fprintf(stderr,
            "#Input error: \"end\" without \"function\".\n",n);
          exit(1);
        }
     }
     if (*delim=='\'') {
        n=getc(rfile);
        if (n=='\\') n=getc(rfile);
        n=getc(rfile);
        if (n!='\'') {
          fprintf(stderr,
            "#Input error: ' expected.\n",n);
          exit(1);
        }
     }
     else if (*delim=='\"') {
         do {
           n=getc(rfile);
           while (n=='\\') {
              n=getc(rfile); n=getc(rfile);
           }
           if (n=='\n' || n==EOF) {
              fprintf(stderr, "#Input error: newline or EOF in string.\n",n);
              exit(1);
            }
         } while (n!='\"');
     }
     else if (*delim=='(' || *delim=='[' || *delim=='{') /*}*/
        br_rec[br_ct++] = *delim;
     else if (*delim==')' && br_ct>0) {
        if (br_rec[br_ct-1]=='(')
           br_ct--;
        else{
          fprintf(stderr,
            "#Input error: ')' without '('.\n",n);
          exit(1);
        }
     }
     else if (*delim==']') {
        if (br_ct>0 && br_rec[br_ct-1]=='[')
           br_ct--;
        else{
          fprintf(stderr,
            "#Input error: ']' without '['.\n",n);
          exit(1);
        }
     }
     /*{*/
     else if (*delim=='}') {
        if (br_ct>0 && br_rec[br_ct-1]=='{') /*}*/
           br_ct--;
        else{
          fprintf(stderr,
            /*{*/  "#Input error: '}' without '{'.\n",n);  /*}*/
          exit(1);
        }
     }
     else if (br_ct==0 && (*delim==',' || *delim==')' || *delim==';'))
        break;
     if (br_ct>=1024) {
         fprintf(stderr,
           "#Input error: Bracket level too deep.\n",n);
         exit(1);
     }
  }

}

void
read_ident(rfile,ident,delim,inv)
        FILE * rfile;
        char * ident;
        int * delim;
	boolean inv;
/* Read next identifier, field name, etc.
 * if inv is true, it is allowed to end ^-1.
 */
{ int n;
  char * ptr=ident;
  while ((n=read_char(rfile))==' ');
  while (n != ' ' && !isdelim(n)){
    *(ptr++) = n;
    n=read_char(rfile);
  }
  if (n==' ')
    while ((n=read_char(rfile))==' ');
  if (!isdelim(n)){
    fprintf(stderr,
        "#Input error: delimiter expected to terminate ident (not %c).\n",n);
    exit(1);
  }
  if (inv && n=='^') {
  /* With some reluctance, I allow a generatro to be called {ident}^-1 */
    *(ptr++)='^';
    check_next_char(rfile,'-'); *(ptr++)='-';
    check_next_char(rfile,'1'); *(ptr++)='1';
    read_delim(rfile,&n);
  }
  *ptr = '\0';
  *delim = n;
}

void
read_int(rfile,integ,delim)
        FILE * rfile;
        int * integ;
        int * delim;
/* Read next integer - may be empty, when 0 is returned as *integ.
 * NOTE integer can be terminated by a '.' as well as a delimiter
 */
{ int n, value=0;
  boolean neg=FALSE;
  while ((n=read_char(rfile))==' ');
  do{
    if (n=='-' && value==0)
      neg=TRUE;
    else if (isdigit(n))
      value = 10*value+n-'0';
    else if (n==' ' || n== '.' || isdelim(n))
      break;
    else{
      fprintf(stderr,"#Input error: integer expected.\n");
      exit(1);
    }
    n=read_char(rfile);
  } while (1);
  if (n==' ')
    while ((n=read_char(rfile))==' ');
  if (n!='.' && !isdelim(n)){
    fprintf(stderr,
       "#Input error: delimiter expected to terminate integer (not %c).\n",n);
    exit(1);
  }
  if (neg)
    value = -value;
  *integ = value;
  *delim = n;
}


void
read_string(rfile,string,delim)
        FILE * rfile;
        char * string;
        int * delim;
/* Read next string (enclosed in quotes) - if delim comes first, return
 * empty string as string.
 */
{ int n;
  char * ptr=string;
  while ((n=read_char(rfile))==' ');
  if (n=='\"') {
    do {
      n=getc(rfile);
      while (n=='\\') {
         n=getc(rfile); *(ptr++)=n; n=getc(rfile);
      }
      if (n=='\n' || n==EOF) {
         fprintf(stderr, "#Input error: newline or EOF in string.\n",n);
         exit(1);
       }
      if (n!='\"')
        *(ptr++) = n;
      else
        break;
    } while (1);
    read_delim(rfile,delim);
/* We want the next delimiter after the terminating quotes */
  }
  else if (!isdelim(n)){
    fprintf(stderr,
         "#Input error: delimiter expected in read_string (not %c).\n",n);
    exit(1);
  }
  else
    *delim = n;
  *ptr = '\0';
}

void
process_names(name,num_names)
	char **name;
	int num_names;
/* name is a list of num_names names (probbaly names of monoid generators).
 * First they are checked for validity - they must start with a letter or
 * underscore, and all characters must be alphanumeric '_' or '.'.
 * (or they can end in ^-1).
 * They are then checked to see if they comply with one of the two
 * special formats a) and b) described at the rop of the file rwsio.h.
 * If so, then the external variables algno and gen_no are set
 * accordingly. These are used by the next function read_next_word for
 * fast reading of words in the generators.
 * Otherwise, algno is set to -1, and words are located in the list
 * by a simple search.
 */
{ int i, j, l;
  char *gen, *gen1, *ptr;

  for (i=0;i<=127;i++)
    gen_no[i] = 0;
  algno = 0;
  if (num_names==0)
    return;
  for (i=1;i<=num_names;i++){
    gen = name[i];
    if (strlen(gen)==0){
      fprintf(stderr,
           "#Input error: a generator name has length 0.\n");
      exit(1);
    }
    if (strlen(gen)>1)
      algno = -1;
    if (!isalpha(gen[0]) && gen[0]!='_'){
      fprintf(stderr,
           "#Input error: generators should start with a letter or `_'.\n");
      exit(1);
    }
    if (algno==0) {
      if (gen_no[gen[0]] != 0){
        fprintf(stderr,"#Input error: repeated generator name.\n");
        exit(1);
      }
      gen_no[gen[0]]=i;
    }
  }
  if (algno == 0)
    return; /* all generators are single letters or underscore. */

  for (i=1;i<=num_names;i++) {
    gen = name[i];
    l = strlen(gen);
    for (j=0;j<l;j++) if (!isvalid(gen[j])){
      if (j!=l-3 || gen[j]!='^' || gen[j+1]!='-' || gen[j+2]!='1') {
        fprintf(stderr,
 "#Input error: generator %s: names must be alphanumeric, '_', or '.'.\n",gen);
        exit(1);
      }
      else break;
    }
    for (j=1;j<i;j++) if (strcmp(gen,name[j])==0) {
      fprintf(stderr,"#Input error: repeated generator name.\n");
      exit(1);
    }
  }
    
/* Try to locate prefix  for Case b). */
  gen1 = name[1];
  ptr = gen1 + strlen(gen1) - 1;
  if (!isdigit(*ptr)) { /* not Case b) */
    algno = -1;
    return;
  }
  while (isdigit(*ptr))
    ptr--;
  algno = ptr-gen1+1;
  if (atoi(gen1+algno) > MAXUCHAR){ /* numerical subscript too large */
    algno = -1;
    return;
  }

  gen_no[atoi(gen1+algno)] = 1;
  for (i=2;i<=num_names;i++){
    gen = name[i];
    if (strlen(gen)<algno+1){ /* not Case b) */
      algno = -1;
      return;
    }
    for (j=0;j<algno;j++) if (gen[j]!=gen1[j]){ /* not Case b) */
      algno = -1;  
      return;
    }
    for (j=algno;j<strlen(gen);j++) if (!isdigit(gen[j])){ /* not Case b) */
      algno = -1;  
      return;
    }
    if (atoi(gen+algno) > MAXUCHAR){ /* numerical subscript too large */
      algno = -1;
      return;
    }
    gen_no[atoi(gen+algno)] = i;
  }
}

void
read_word(rfile,gen_word,end_word,delim,name,num_names,check)
        FILE * rfile;
        char * gen_word;
        char * end_word;
        int * delim;
	char **name;
	int num_names;
        boolean check;
/* A word is read into gen_word.
   It is assumed that space is already allocated for the word beginning at
   gen_word and ending at end_word.
   If the word is too long, the process aborts.
   The word must be empty or equal to "IdWord" (equivalent) or have the form
   t1*t2*...tr (for some r>=1), where each ti is either the name of
   a generator, or of form <gen-name>^<integer>.
   2/6/95 - brackets are now allowed up to level 8, so
            expressions like (x*(y*z)^3)^9  are allowed.
   If <integer> is negative, then there must be a generator called
   <gen-name>^-1.
   The word is translated and expanded into a string of generator
   numbers and stored as an ASCII string in gen_word.
   The external array gen_no, and the external integer algno are used
   for the translation - see comment in rwsio.h for explanation
   If check is set true, then the validity of the word is checked -
   using the list "name" of generator names.
   Otherwise it is processed as fast as possible.
*/
{ int   i, g, n, delimchar, bracketlevel;
  char  *ptr, *ptrr, *ptre, *bracketptr[9];
   ptr =  gen_word;
   bracketlevel=0;
   do{
    read_ident(rfile,buffer,&delimchar,FALSE);
    if (delimchar=='(') {
      if (strlen(buffer)!=0) {
        fprintf(stderr,"Input error: '(' in wrong position in word.\n");
        exit(1);
      }
      bracketlevel++;
      if (bracketlevel > 8) {
        fprintf(stderr,"Input error: at most 8 levels of brackets allowed.\n");
        exit(1);
      }
      bracketptr[bracketlevel] = ptr;
      continue;
    }
    if (ptr==gen_word && (strcmp(buffer,"")==0 || strcmp(buffer,"IdWord")==0))
      break;
    if (algno == -1) {
      g = 0;
      for (i=1;i<=num_names;i++) if (strcmp(buffer,name[i])==0) {
        g = i;
        break;
      }
      if (g==0) {
        fprintf(stderr,"#Input error: unknown generator in word.\n");
        exit(1);
      }
    }
    else {
      g = (algno==0) ? gen_no[*buffer] : gen_no[atoi(buffer+algno)];
      if (check && (g==0 || strcmp(buffer,name[g])) != 0){
        fprintf(stderr,"#Input error: invalid entry in word.\n");
        exit(1);
      }
    }
    if (delimchar=='^'){
      read_int(rfile,&n,&delimchar);
      if (delimchar!='*' && delimchar!=']' && delimchar!=','
		&& delimchar!=';'  && delimchar!=')'){
        fprintf(stderr,"#Input error: ',' '*', ')'  or ']' expected.\n");
        exit(1);
      }
      if (n<0){
         /* This is only allowed if we have a generator ending in ^-1 */
         g=0;
         if (algno == -1) {
           sprintf(buffer+strlen(buffer),"^-1");
           for (i=1;i<=num_names;i++) if (strcmp(buffer,name[i])==0) {
             g = i;
             break;
           }
         }
         if (g==0) {
           fprintf(stderr,
   "#Input error: unknown generator or illegal negative power in word.\n");
           exit(1);
         }
         n = -n;
      }
      for (i=1;i<=n;i++) {
        *(ptr++) = g;
        if (ptr>end_word) {
          fprintf(stderr,
           "The word being read is too long. Increase maxreducelen.\n");
            exit(1);
        }
      }
    }
    else if (delimchar=='*' || delimchar==']' || delimchar==',' ||
		delimchar==';' || delimchar==')'){
      *(ptr++) = g;
      if (ptr>end_word) {
        fprintf(stderr,
         "The word being read is too long. Increase maxreducelen.\n");
          exit(1);
      }
    }
    else{
      fprintf(stderr,"#Input error: ',' '^' '*' ';' or ']' expected.\n");
      exit(1);
    }
    while (delimchar==')'){
      read_delim(rfile,&delimchar);
      while (delimchar==')') {
        bracketlevel--;
        read_delim(rfile,&delimchar);
      }
      if (bracketlevel<=0) {
		fprintf(stderr,"Input error: ')' with no matching '('.\n");
		exit(1);
      }
      if (delimchar=='^') {
        read_int(rfile,&n,&delimchar);
        if (delimchar!='*' && delimchar!=']'&&delimchar!=','
		&& delimchar!=';' && delimchar!=')'){
          fprintf(stderr,"#Input error: ',' '*' ')' or ']' expected.\n");
          exit(1);
        }
        if (n<0){
            fprintf(stderr,
               "#Error: Negative powers not allowed in monoid words.\n",g);
            exit(1);
        }
        ptre = ptr-1;
        for (i=1;i<n;i++) {
          ptrr = bracketptr[bracketlevel];
          while (ptrr <= ptre) {
            *(ptr++) = *(ptrr++);
            if (ptr>end_word) {
              fprintf(stderr,
               "The word being read is too long. Increase maxreducelen.\n");
                exit(1);
            }
          }
        }
      }
      else if (delimchar!='*'&&delimchar!=']'&&delimchar!=','&&delimchar!=';'){
        fprintf(stderr,"#Input error: ',' '^' '*' ';' or ']' expected.\n");
        exit(1);
      }
      bracketlevel--;
    }
  } while (delimchar!=',' && delimchar!=']' && delimchar!=';');

  if (bracketlevel != 0) {
    fprintf(stderr,"#Input error: brackets do not balance in word.\n");
    exit(1);
  }
  *delim = delimchar;
  *ptr = '\0';
}

void
printbuffer(wfile)
	FILE *wfile;
/* prints contents of buffer, followed by new-line to wfile
 * and then clears buffer.
 */
{ fprintf(wfile,"%s\n",buffer);
  *buffer = '\0';
}

void
add_to_buffer(n,w)
	int n;
	char *w;
/* w should be a string.
 * Add at least n characters to the end of the string buffer, with w at the end,
 * padding with spaces at beginning
 */
{ int i,nsp;
  nsp = n-strlen(w);
  for (i=1;i<=nsp;i++)
    strcat(buffer," ");
  strcat(buffer,w);
}

int
add_word_to_buffer(wfile,word,symbols)
	FILE *wfile;
	char *word;
	char **symbols;
/* word is a string of chars. to be printed as a word in GAP syntax.
 * If char number i appears in the string, then
 * the char is to be printed as the string symbols[i].
 * The buffer contains the line to be printed, and the word is
 * appended to the end of the current contents of the buffer.
 * New lines are printed in mid-word if necessary, but not at the
 * end, since the calling function may need to append more.
 * The returned value is the number of newlines inserted.
 */
{ int offset, pow, g, len, lg, nln=0;
  char *w, sg[128];
  boolean first;
  offset=strlen(buffer)+2; /* remember where word begins on the line,
                            * in case we need to add new lines. */
  w = word;
  if (*w == 0)
    add_to_buffer(0,"IdWord");
  first = TRUE;
  while (*w){
    g = *(w++);
    pow = 1;
    while (*w == g){
      pow++; w++;
    }
    len = first ? 0 : 1;
    strcpy(sg,symbols[g]);
    len += strlen(sg);
    if (pow > 1){
     len += (1 + int_len(pow));
    }
    if (strlen(buffer) + len >= 76){ /* new line needed */
      printbuffer(wfile);
      add_to_buffer(offset,"");
      nln++;
    }
    if (!first)
      add_to_buffer(0,"*");
    /* Check if we have a power of a generator ending with ^-1 */
    lg = strlen(sg);
    if (pow>1 && lg>3 && sg[lg-3]=='^' && sg[lg-2]=='-' && sg[lg-1]=='1') {
      sg[lg-3]='\0'; pow = -pow;
    }
    add_to_buffer(0,sg);
    if (pow > 1 || pow < -1)
      sprintf(buffer+strlen(buffer),"^%d",pow);
    first = FALSE;
  }
  return nln;
}

add_iword_to_buffer(wfile,word,symbols)
	FILE *wfile;
	int *word;
	char **symbols;
/* This is the same as add_word_to_buffer, except that "word" is a string of
 * integers instead of characters.
 */
{ int offset, pow, g, lg, len, nln=0;
  int *w;
  char sg[128];
  boolean first;
  offset=strlen(buffer)+2; /* remember where word begins on the line,
                            * in case we need to add new lines. */
  w = word;
  if (*w == 0)
    add_to_buffer(0,"IdWord");
  first = TRUE;
  while (*w){
    g = *(w++);
    pow = 1;
    while (*w == g){
      pow++; w++;
    }
    len = first ? 0 : 1;
    strcpy(sg,symbols[g]);
    len += strlen(sg);
    if (pow > 1){
     len += (1 + int_len(pow));
    }
    if (strlen(buffer) + len >= 76){ /* new line needed */
      printbuffer(wfile);
      add_to_buffer(offset,"");
      nln++;
    }
    if (!first)
      add_to_buffer(0,"*");
    /* Check if we have a power of a generator ending with ^-1 */
    lg = strlen(sg);
    if (pow>1 && lg>3 && sg[lg-3]=='^' && sg[lg-2]=='-' && sg[lg-1]=='1') {
      sg[lg-3]='\0'; pow = -pow;
    }
    add_to_buffer(0,sg);
    if (pow > 1 || pow < -1)
      sprintf(buffer+strlen(buffer),"^%d",pow);
    first = FALSE;
  }
  return nln;
}

int
int_len(n)
	int n;
/* The length of the  integer  n */
{ int l=0;
  if (n < 0){
    n = -n;
    l = 1;
  }
  while (n != 0){
    l++; n = n/10;
  }
  return l;
}

boolean
is_int(x)
        char *x;
/* returns true if the string x is an integer */
{ int i, l;
  l = strlen(x);
  if (l==0 || (l==1 && x[0]=='-'))
    return FALSE;
  for (i=0;i<l;i++)
    if (!isdigit(x[i]) && (i!=0 || x[i]!='-'))
      return FALSE;
  return TRUE;
}
