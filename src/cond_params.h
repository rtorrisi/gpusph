/*  Copyright 2014 Alexis Herault, Giuseppe Bilotta, Robert A. Dalrymple, Eugenio Rustico, Ciro Del Negro

    Istituto Nazionale di Geofisica e Vulcanologia
        Sezione di Catania, Catania, Italy

    Università di Catania, Catania, Italy

    Johns Hopkins University, Baltimore, MD

    This file is part of GPUSPH.

    GPUSPH is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GPUSPH is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GPUSPH.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _COND_PARAMS_H
#define _COND_PARAMS_H

/*! \file
   Some kernels are very complex and require a different set of parameters in
   some specializations.

   All of this should be managed as automagically as possible while trying to
   rely only on the most readable features of C++ and, if possible, using the
   preprocessor as little as possible.

   To this end, we set up a mechanism that allows us to build structure
   templates in which the number and quality of members depends on the
   specialization.

   These template structures depend on std::conditional to select which
   struct members are allowed in each specialization of the options.
   This is part of the C++11 stdlib, but it can be easily implemented in older
   C++ revisions too.
*/

#include "cpp11_missing.h" // conditional<>

/* The general idea is that each group of members of the structure is defined as
   a specific (non-template, usually) structure, and then the actual template
   structure is defined as a derived class of each of the specific (sub) structures,
   if the appropriate template parameters are set. When the template parameter values
   are not correct, an empty structure is included in place of the actual one.
   While the structure itself is empty, it must provide a constructor that acts as
   fallback for each constructor used in the structure it is used as a replacement of.
*/

/*! Empty structure to be used in place of the given typename when the
 * structre inclusion condition is not satisfied.
 */
template<typename>
struct empty_struct
{
	/// Empty constructor
	__host__ __device__ __forceinline__
	empty_struct() {}

	/// Universal constructor
	template<typename ...T1>
	__host__ __device__ __forceinline__
	empty_struct(T1 const&...) {}
};


/*! Inclusion of struct_to_include under a given condition would be achieved by
   deriving the complete class from:
       conditional<boolean_condition, struct_to_include, empty<struct_to_include> >::type
   for example:
       conditional<boundarytype == SA_BOUNDARY, sa_boundary_forces_params, empty<sa_boundary_forces_params> >::type
   which is very verbose, so we define a macro COND_STRUCT(boolean_condition, struct_to_include) to do the job for us
*/

#define COND_STRUCT(some_cond, ...) \
	conditional<bool(some_cond), __VA_ARGS__, empty_struct< __VA_ARGS__ > >::type

#endif // _COND_PARAMS_H

