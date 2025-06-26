/*
 *  config.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

/*
  config.h is automatically generated from config.h.in during the
  configuration process. Please make all changes in config.h.in.
*/

#define NEST_VERSION "3.8.0-post0.dev0"
#define NEST_VERSION_GIT "1"
#define NEST_VERSION_GIT_HASH "166aa9352b1e53c903ab2edcaf65ccdd5154b01c"
#define NEST_VERSION_GIT_BRANCH "MUSIC"
#define NEST_VERSION_GIT_REMOTE "unknown"

// TODO NEST_HOST and NEST_HOSTVENDOR not available with cmake
#define NEST_HOST "arm64-apple-darwin"
#define NEST_HOSTOS "darwin"
#define NEST_HOSTCPU "arm64"
#define NEST_HOSTVENDOR "apple"

#define NEST_INSTALL_PREFIX "/Users/alp/nest-env"
#define NEST_INSTALL_DATADIR "share/nest"
#define NEST_INSTALL_DOCDIR "share/doc/nest"
#define NEST_INSTALL_LIBDIR "lib/nest"

#define MPIEXEC "/Users/alp/nest-env/bin/mpiexec"
#define MPIEXEC_NUMPROC_FLAG "-n"
#define MPIEXEC_MAX_NUMPROCS "8"
#define MPIEXEC_PREFLAGS ""
#define MPIEXEC_POSTFLAGS ""

#define NEST_EXITCODE_ABORT 134
#define NEST_EXITCODE_SEGFAULT 139

/* Name of package */
#define PACKAGE nest

/* tics per ms in simulation */
#define CONFIG_TICS_PER_MS 1000.0

/* tics per step in simulation */
#define CONFIG_TICS_PER_STEP 100

/* Define to 1 if you have a 32 bit architecture */
/* #undef HAVE_32BIT_ARCH */

/* define if the compiler does not include *.h headers ISO conformant */
/* #undef HAVE_ALPHA_CXX_STD_BUG */

/* define if the compiler ignores cmath makros */
/* #undef HAVE_CMATH_MAKROS_IGNORED */

/* "Define if expm1() is available" */
/* #undef HAVE_EXPM1 */

/* Is the GNU Science Library available (ver. >= 1.0)? */
#define HAVE_GSL 1

/* "Define if isnan() is available" */
#define HAVE_ISNAN 1

/* "Define if std::isnan() is available" */
#define HAVE_STD_ISNAN 1

/* define for istream */
#define HAVE_ISTREAM 1

/* Havel libltdl, can load dynamic modules */
#define HAVE_LIBLTDL 1

/* libneurosim support enabled? */
/* #undef HAVE_LIBNEUROSIM */

/* Define to 1 if you have usable long long type. */
#define HAVE_LONG_LONG 1

/* Define to 1 if you have u_int46_t type. */
#define HAVE_U_INT64_T 1

/* Define to 1 if you have uint46_t type. */
#define HAVE_UINT64_T 1

/* Define to 1 if you have the <mach/mach.h> header file. */
#define HAVE_MACH_MACH_H 1

/* Define to 1 if you have the <mach-o/dyld.h> header file. */
#define HAVE_MACH_O_DYLD_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* MPI is available. */
#define HAVE_MPI 1

/* Boost is available. */
#define HAVE_BOOST 1

/* MUSIC support enabled? */
/* #undef HAVE_MUSIC */

/* define if M_E is defined */
#define HAVE_M_E 1

/* define if M_PI is defined */
#define HAVE_M_PI 1

/* "Define if NAN is available" */
#define HAVE_NAN 1

/* "Define if std::nan(nullptr) is available" */
#define HAVE_STD_NAN 1

/* define for ostream */
#define HAVE_OSTREAM 1

/* Use GNU libreadline */
#define HAVE_READLINE 1

/* define if the compiler ignores symbolic signal names in signal.h */
/* #undef HAVE_SIGUSR_IGNORED */

/* define for sstream */
#define HAVE_SSTREAM 1

/* define if the compiler fails on static template member declaration */
/* #undef HAVE_STATIC_TEMPLATE_DECLARATION_FAIL */

/* define for STL vector capacity base unity */
#define HAVE_STL_VECTOR_CAPACITY_BASE_UNITY 1

/* define for STL vector capacity doubling strategy */
#define HAVE_STL_VECTOR_CAPACITY_DOUBLING 1

/* Tics per millisecond specified? */
#define HAVE_TICS_PER_MS 1

/* Tics per step specified? */
#define HAVE_TICS_PER_STEP 1

/* define if the compiler fails with ICE */
/* #undef HAVE_XLC_ICE_ON_USING */

/* Is SIONlib available? */
/* #undef HAVE_SIONLIB */

/* Is HDF5 available? */
/* #undef HAVE_HDF5 */

/* Is mpi4py available? */
#define HAVE_MPI4PY 1

/* Do Random123 generators work? */
#define HAVE_RANDOM123 1

/* Bit counts for Target members */
#define TARGET_BITS_SPLIT 0
#define TARGET_BITS_SPLIT_STANDARD 0
#define TARGET_BITS_SPLIT_HPC 1

/* Whether to enable detailed NEST internal timers */
/* #undef TIMER_DETAILED */

/* Whether to do full logging */
/* #undef ENABLE_FULL_LOGGING */

#endif // #ifndef CONFIG_H
