/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

==============================================================================*/
/******************************************************************************

    Copyright (C) 2009 William Hart

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "ulong_extras.h"

int main(void)
{
   int i, result;
   printf("mulmod2_preinv....");
   fflush(stdout);

   for (i = 0; i < 1000000; i++)
   {
      mp_limb_t a, b, d, r1, r2, q, norm, p1, p2, dinv;
      
      d = n_randtest_not_zero();
      a = n_randint(d);
      b = n_randint(d);
      
      dinv = n_preinvert_limb(d);

      r1 = n_mulmod2_preinv(a, b, d, dinv);

      umul_ppmm(p1, p2, a, b);
      p1 %= d;
      udiv_qrnnd(q, r2, p1, p2, d); 
      
      result = (r1 == r2);
      if (!result)
      {
         printf("FAIL:\n");
         printf("a = %lu, b = %lu, d = %lu, dinv = %lu\n", a, b, d, dinv); 
         printf("r1 = %lu, r2 = %lu\n", r1, r2);
         abort();
      }
   }

   printf("PASS\n");
   return 0;
}
