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

=============================================================================*/
/******************************************************************************

    Copyright (C) 2009 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("is_squarefree....");
    fflush(stdout);

    flint_randinit(state);

    /* Check that polynomials of degree <= 1 are square-free */
    for (i = 0; i < 50 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t f;

        fmpz_poly_init(f);
        fmpz_poly_randtest(f, state, n_randint(state, 2), 100);

        result = (fmpz_poly_is_squarefree(f));
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_debug(f), printf("\n");
            abort();
        }

        fmpz_poly_clear(f);
    }

    /* Check that a^2 f is not square-free */
    for (i = 0; i < 20 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, f;

        fmpz_poly_init(a);
        fmpz_poly_randtest_not_zero(a, state, n_randint(state, 20) + 1, 40);
        if (a->length < 2)
        {
            fmpz_poly_clear(a);
            continue;
        }
        fmpz_poly_init(f);
        fmpz_poly_randtest_not_zero(f, state, n_randint(state, 20) + 1, 40);

        fmpz_poly_mul(a, a, a);
        fmpz_poly_mul(f, a, f);

        result = (!fmpz_poly_is_squarefree(f));
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_debug(f), printf("\n");
            abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(f);
    }

    /* Check that f + N*(x^M + 1) is square-free, for N >> f, M > deg(f) */
    for (i = 0; i < 20 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, f;
        fmpz_t N;

        fmpz_poly_init(a);
        fmpz_poly_set_coeff_si(a, 0, 1L);
        fmpz_poly_set_coeff_si(a, n_randint(state, 20), 1L);
        if (a->length < 2)
        {
            fmpz_poly_clear(a);
            continue;
        }
        fmpz_poly_init(f);
        fmpz_poly_randtest(f, state, a->length - 2, 40);

        fmpz_init_set_ui(N, 1UL);
        fmpz_mul_2exp(N, N, 45 + a->length);

        fmpz_poly_scalar_mul_fmpz(a, a, N);
        fmpz_poly_add(f, a, f);

        result = fmpz_poly_is_squarefree(f);
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_poly_debug(f), printf("\n");
            abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(f);
        fmpz_clear(N);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
