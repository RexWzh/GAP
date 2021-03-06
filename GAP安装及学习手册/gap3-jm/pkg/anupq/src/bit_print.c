#include <stdlib.h>
#include <stdio.h>
#define BITES_IN_INT 8 * sizeof (int)

/* print out the bit representation of the integer, v */

void bit_print (v)
int v;
{   
   register int i;
   int mask = 1 << (BITES_IN_INT - 1);

   for (i = 1; i <= BITES_IN_INT; ++i) {
      putchar (((v & mask) == 0) ? '0': '1');
      v <<= 1;
      if (i % 8 == 0 && i != BITES_IN_INT)
	 putchar (' ');
   }
   printf ("\n");
}
