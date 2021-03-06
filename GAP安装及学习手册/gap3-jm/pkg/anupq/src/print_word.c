#include "pq_defs.h"
#include "pcp_vars.h"

/* print out a word of a pcp presentation */

void print_word ( int ptr, struct pcp_vars *pcp)
{
#include "define_y.h"

   register int gen, exp;
   register int i;
   register int count;
#include "access.h"

   if (ptr == 0)
      printf (" IDENTITY\n");
   else if (ptr > 0)
      printf (" .%d\n", ptr);
   else {
      ptr = -ptr + 1;
      count = y[ptr];
      for (i = 1; i <= count; i++) {
	 exp = FIELD1 (y[ptr + i]);
	 gen = FIELD2 (y[ptr + i]);
	 printf (" .%d", gen);
	 if (exp != 1)
	    printf ("^%d", exp);
      }
      printf ("\n");
   }
}

/* print out exponent-vector with base address cp, first 
   converting it to string stored at str for printing */

void setup_word_to_print ( char *type, int cp, int str, struct pcp_vars *pcp)
{
#include "define_y.h"

#if defined (GROUP) 
   vector_to_string (cp, str, pcp);

   /* may need to set up pointer to this string at a later stage */
   ++str;

   printf ("The %s is ", type);
   if (y[str] == 0) 
      print_word (0, pcp);
   else
      print_word (-str + 1, pcp);

#endif

#if defined (LIE)
   str = vector_to_string (cp, str, pcp);

   printf ("The %s is ", type);
   if (str == 0) 
      print_word (0, pcp);
   else
      print_word (str, pcp);

#endif
}
