/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef ROCKETCOREPLATFORM_H
#define ROCKETCOREPLATFORM_H

#include "Config.h"

#if defined __WIN32__ || defined _WIN32
	#define ROCKET_PLATFORM_WIN32
	#define ROCKET_PLATFORM_NAME "win32"
	#if !defined(__MINGW32__)
		#pragma warning(disable:4355)
	#endif
#elif defined __APPLE_CC__
	#define ROCKET_PLATFORM_UNIX
	#define ROCKET_PLATFORM_MACOSX
	#define ROCKET_PLATFORM_NAME "macosx"
#else
	#define ROCKET_PLATFORM_UNIX
	#define ROCKET_PLATFORM_LINUX
	#define ROCKET_PLATFORM_NAME "linux"
#endif

#if !defined NDEBUG && !defined ROCKET_DEBUG
	#define ROCKET_DEBUG
#endif

#if defined __LP64__ || defined _M_X64 || defined __MINGW64__ || defined _LP64
    #define ROCKET_ARCH_64
#else
    #define ROCKET_ARCH_32
#endif


#if defined(ROCKET_PLATFORM_WIN32) && !defined(__MINGW32__)
	// alignment of a member was sensitive to packing
	#pragma warning(disable : 4121)

	// <type> needs to have dll-interface to be used by clients
	#pragma warning(disable : 4251)

	// assignment operator could not be generated
	#pragma warning(disable : 4512)

	// <function> was declared deprecated
	#pragma warning(disable : 4996)

	#pragma warning(disable : 4275)

	#if !defined _CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE
	#endif
#endif

// Wraps unused variables in methods or functions to avoid compiler warnings.  This should
// be wrapped around the name of all parameters that are wrapped with ROCKET_UNUSED_PARAMETER
// to cover warnings generated by non-llvm/gcc style compilers that can't be covered with
// ROCKET_UNUSED_PARAMETER
#if defined __llvm__
//#  define ROCKET_UNUSED(x) UNUSED_ ## x __unused
#  define ROCKET_UNUSED(x)
#elif defined __GNUC__
//#  define ROCKET_UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#  define ROCKET_UNUSED(x)
#else
#  define ROCKET_UNUSED(x) (void)(UNUSED_ ## x)
#endif

// Wraps unused parameter names in method or function declarations.  When used, the first lines
// of the function should contain a matching ROCKET_UNUSED call with the name of the function
// as well to cover compilers with no-op ROCKET_UNUSED_PARAMETER macros.
#if defined __llvm__
#  define ROCKET_UNUSED_PARAMETER(x) UNUSED_ ## x __attribute__((unused))
#elif defined __GNUC__
#  define ROCKET_UNUSED_PARAMETER(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define ROCKET_UNUSED_PARAMETER(x) UNUSED_ ## x
#endif

// ROCKET_UNUSED_ASSERT_PARAMETERS wraps method parameters which are not used in the method other than
// by a ROCKET_ASSERT check.  This safely deals with debug versus release mode configurations
// and will warn if the parameter starts being used when compiled with GCC
#ifdef ROCKET_DEBUG
   // In this case, the parameter is used by a ROCKET_ASSERT test, so we just pass through as is
#  define ROCKET_UNUSED_ASSERT_PARAMETER(x) x
#  define ROCKET_UNUSED_ASSERT(x)
#else
   // If not in DEBUG builds, this parameter is unused, mark it as such
#  if defined __llvm__
#    define ROCKET_UNUSED_ASSERT_PARAMETER(x) UNUSED_ ## x __attribute__((unused))
#    define ROCKET_UNUSED_ASSERT(x)
#  elif defined __GNUC__
#    define ROCKET_UNUSED_ASSERT_PARAMETER(x) UNUSED_ ## x __attribute__((__unused__))
#    define ROCKET_UNUSED_ASSERT(x)
#  else
#    define ROCKET_UNUSED_ASSERT_PARAMETER(x) UNUSED_ ## x
#    define ROCKET_UNUSED_ASSERT(x) (void)(UNUSED_ ## x)
#  endif
#endif

// Wraps functions which are not referenced or exported to avoid compiler warnings
#if defined __llvm__
#  define ROCKET_UNUSED_FUNCTION(x) __attribute__((unused)) UNUSED_ ## x
#elif defined __GNUC__
#  define ROCKET_UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define ROCKET_UNUSED_FUNCTION(x) UNUSED_ ## x
#endif

// Squelchs warnings for unused enums in switch statements, this should only be used for special values
// that are known to NEVER be used.
#define ROCKET_UNUSED_SWITCH_ENUM(x) \
  case x: \
    ROCKET_ERRORMSG("Switch case for unhandled ENUM has been hit!  This shouldn't happen!  ENUM Name: " # x); \
    break;

#endif
