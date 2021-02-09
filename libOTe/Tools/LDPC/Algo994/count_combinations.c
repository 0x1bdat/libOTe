/*

   MinDistance.
   Software package with several fast scalar, vector, and parallel
   implementations for computing the minimum distance of a random linear code.

   Copyright (C) 2017  Fernando Hernando (carrillf@mat.uji.es)
   Copyright (C) 2017  Francisco Igual (figual@ucm.es)
   Copyright (C) 2017  Gregorio Quintana (gquintan@uji.es)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "libOTe/Tools/LDPC/Algo994/count_combinations.h"


// ============================================================================
// Declaration of local variables.

static uint64_t _numCombinations;

// ============================================================================
void init_num_combinations_in_count_combinations() {
//
// Initialize the current number of combinations.
//
  _numCombinations = 0;
  #pragma omp flush( _numCombinations )

}

// ============================================================================
void print_num_combinations_in_count_combinations() {
//
// Print the number of checked combinations.
//
  #pragma omp flush( _numCombinations )
  printf( "Number of total combinations evaluated: %" PRId64 "\n\n",
          _numCombinations );
}

// ============================================================================
void add_num_combinations_in_count_combinations( int nc ) {
//
// Add the argument "nc" to the current number of combinations.
//
  #pragma omp critical
    _numCombinations += ( uint64_t ) nc;
}

