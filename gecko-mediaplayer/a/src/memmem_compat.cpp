/* This file implements memmem, a function to find the first occurrence
   of the contents of a memory area in another memory area.
   Copyright (C) 2003 Martin Dickopp
  
   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
   USA.  */



#include <stddef.h>
#include <stdlib.h>


/* This function implements the Boyer-Moore algorithm.
   It is assumed that chars have eight bits.  */
void *memmem_compat(const void *haystack, const size_t haystack_len,
                    const void *needle, const size_t needle_len)
{
    const unsigned char *const haystack_endptr = (const unsigned char *) haystack + haystack_len;
    const unsigned char *const needle_endptr = (const unsigned char *) needle + needle_len;
    const unsigned char *haystack_shifting_ptr;

    size_t *shift_good_suffix;
    size_t shift_last_occurrence[256];


    if (needle_len > haystack_len)
        return 0;

    haystack_shifting_ptr = (const unsigned char *) haystack + needle_len;


    /* Compute good suffix function.  */
    shift_good_suffix = (size_t *) malloc(2 * needle_len * sizeof *shift_good_suffix);
    if (shift_good_suffix != 0) {
        const unsigned char *needle_ptr;
        size_t i, j;

        shift_good_suffix[0] = 0;
        needle_ptr = (const unsigned char *) needle + 1;
        for (i = 1, j = 0; i < needle_len; ++i) {
            while (j > 0 && ((const unsigned char *) needle)[j] != *needle_ptr)
                j = shift_good_suffix[j - 1];
            if (((const unsigned char *) needle)[j] == *needle_ptr)
                ++j;
            shift_good_suffix[i] = j;
            ++needle_ptr;
        }

        shift_good_suffix[needle_len] = 0;
        needle_ptr = (const unsigned char *) needle + needle_len - 1;
        for (i = 1, j = 0; i < needle_len; ++i) {
            --needle_ptr;
            while (j > 0 && ((const unsigned char *) needle)[needle_len - 1 - j] != *needle_ptr)
                j = shift_good_suffix[needle_len - 1 + j];
            if (((const unsigned char *) needle)[needle_len - 1 - j] == *needle_ptr)
                ++j;
            shift_good_suffix[needle_len + i] = j;
        }

        for (i = 0; i < needle_len; ++i)
            shift_good_suffix[i] = needle_len - shift_good_suffix[i];

        for (i = 0; i < needle_len; ++i) {
            j = needle_len - 1 - shift_good_suffix[needle_len + i];
            if (shift_good_suffix[j] > i + 1 - shift_good_suffix[needle_len + i])
                shift_good_suffix[j] = i + 1 - shift_good_suffix[needle_len + i];
        }
    }


    /* Compute last occurence function.  */
    {
        const unsigned char *needle_ptr = (const unsigned char *) needle;
        size_t i;

        for (i = 0; i < 256; ++i)
            shift_last_occurrence[i] = 0;
        for (i = 0; i < needle_len; ++i)
            shift_last_occurrence[*needle_ptr++] = i + 1;
    }


    /* Matching algorithm.  */
    while (haystack_shifting_ptr <= haystack_endptr) {
        const unsigned char *haystack_ptr = haystack_shifting_ptr;
        const unsigned char *needle_ptr = needle_endptr;
        size_t len = needle_len;

        while (len > 0 && *--haystack_ptr == *--needle_ptr)
            --len;

        if (len == 0) {
            if (shift_good_suffix != 0)
                free(shift_good_suffix);
            return (void *) haystack_ptr;
        }

        {
            const size_t shift1 = shift_good_suffix != 0 ? shift_good_suffix[len - 1] : 1;
            const size_t shift2 = (len > shift_last_occurrence[*haystack_ptr]
                                   ? len - shift_last_occurrence[*haystack_ptr] : 1);

            haystack_shifting_ptr += shift1 > shift2 ? shift1 : shift2;
        }
    }


    if (shift_good_suffix != 0)
        free(shift_good_suffix);
    return 0;
}
