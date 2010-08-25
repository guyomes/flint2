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

    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpz_poly.h"

void
_fmpz_poly_compose_horner(fmpz * res, const fmpz * poly1, long len1, 
                                      const fmpz * poly2, long len2)
{
    if (len1 == 1L)
    {
        fmpz_set(res, poly1);
    }
    else
    {
        long i = len1 - 1L, lenr;
        const long alloc = ((len1 - 1L) * (len2 - 1L) + 1L);
        fmpz * t = _fmpz_vec_init(alloc);
        
        /*
           Perform the first two steps as one, 
             "res = a(m) * poly2 + a(m-1)".
         */
        {
            lenr = len2;
            _fmpz_vec_scalar_mul_fmpz(res, poly2, len2, poly1 + i);
            i--;
            fmpz_add(res, res, poly1 + i);
        }
        while (i > 0L)
        {
            i--;
            _fmpz_poly_mul(t, res, lenr, poly2, len2);
            lenr += len2 - 1L;
            _fmpz_poly_add(res, t, lenr, poly1 + i, 1L);
        }
        
        _fmpz_vec_clear(t, alloc);
    }
}

void
fmpz_poly_compose_horner(fmpz_poly_t res, 
                              const fmpz_poly_t poly1, const fmpz_poly_t poly2)
{
    const long len1 = poly1->length;
    const long len2 = poly2->length;
    long lenr;
    
    if (len1 == 0L)
    {
        fmpz_poly_zero(res);
        return;
    }
    if (len1 == 1L || len2 == 0L)
    {
        fmpz_poly_fit_length(res, 1);
        fmpz_set(res->coeffs, poly1->coeffs);
        _fmpz_poly_set_length(res, 1);
        _fmpz_poly_normalise(res);
        return;
    }
    
    lenr = (len1 - 1L) * (len2 - 1L) + 1L;
    
    if ((res != poly1) && (res != poly2))
    {
        fmpz_poly_fit_length(res, lenr);
        _fmpz_poly_compose_horner(res->coeffs, poly1->coeffs, len1, 
                                                          poly2->coeffs, len2);
        _fmpz_poly_set_length(res, lenr);
        _fmpz_poly_normalise(res);
    }
    else
    {
        fmpz_poly_t t;
        fmpz_poly_init2(t, lenr);
        _fmpz_poly_compose_horner(t->coeffs, poly1->coeffs, len1,
                                                          poly2->coeffs, len2);
        _fmpz_poly_set_length(t, lenr);
        _fmpz_poly_normalise(t);
        fmpz_poly_swap(res, t);
        fmpz_poly_clear(t);
    }
}