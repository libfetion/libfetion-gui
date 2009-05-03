/**
* \file common.h
* \author dengdd <dedodong@163.com>
* \date 2009/2/11
* 
* This file includes macro definitions and typedefs that commonly used by libfetion.
*
\verbatim

Copyright (C) 2008 dengdd.

All rights reserved by dengdd.

This file is part of libfetion, a cross-operating-system library to operate China mobile's fetion IM.

\endverbatim
*/

#ifndef _LIBFETION_COMMON_H
#define _LIBFETION_COMMON_H

#ifdef __LIBFETION_LIB__
#include "event.h"

#ifdef WIN32
	#ifdef _WIN32_WCE
		#include "../build/config-winCE.h"
	#else
		#include "../build/config-win32.h"
	#endif 
#else //no WIN32
#include "../fxconfig.h"
#endif  //end #ifdef WIN32

#else  // no __LIBFETION_LIB__
#include "libfetion/fxconfig.h"
#include "libfetion/event.h"
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4005 )
#endif

#define ENABLE(FEATURE) (defined( ENABLE_##FEATURE ) && ENABLE_##FEATURE)

/**
* \defgroup macros_types Macros and data types commonly used
* @{
*/

/**
* \defgroup basic_types Basic data types
* @{
*/

/**
* \var typedef unsigned char Uint8
* \brief A type definition for an 8-bit unsigned character.
*/
typedef unsigned char   Uint8;
/**
* \var typedef signed char Sint8
* \brief A type definition for an 8-bit signed character.
*/
typedef signed char     Sint8;
/**
* \var typedef unsigned short Uint16
* \brief A type definition for a 16-bit unsigned integer.
*/
typedef unsigned short  Uint16;
/**
* \var typedef signed short Sint16
* \brief A type definition for a 16-bit signed integer.
*/
typedef signed short    Sint16;
/**
* \var typedef unsigned int Uint32
* \brief A type definition for a 32-bit unsigned integer.
*/
typedef unsigned int    Uint32;
/**
* \var typedef signed int Sint32
* \brief A type definition for a 32-bit signed integer.
*/
typedef signed int      Sint32;

/* Figure out how to support 64-bit datatypes */
#if !defined(__STRICT_ANSI__)
#if defined(__GNUC__)
#define FETION_HAS_64BIT_TYPE	long long
#endif
#if defined(__CC_ARM)
#define FETION_HAS_64BIT_TYPE	long long
#endif
#if defined(_MSC_VER)
#define FETION_HAS_64BIT_TYPE __int64
#endif
#endif /* !__STRICT_ANSI__ */

/* The 64-bit datatype isn't supported on all platforms */
#ifdef FETION_HAS_64BIT_TYPE

/**
* \var typedef unsigned long long Uint64
* \brief A type definition for a 64-bit unsigned integer.
*
* \warning Only available under GNU C.
*/
typedef unsigned FETION_HAS_64BIT_TYPE Uint64;
/**
* \var typedef signed long long Sint64
* \brief A type definition for a 64-bit signed integer.
*
* \warning Only available under GNU C.
*/
typedef signed FETION_HAS_64BIT_TYPE Sint64;
#else
/* This is really just a hack to prevent the compiler from complaining */
typedef struct {
	Uint32 hi;
	Uint32 lo;
} Uint64, Sint64;
#endif

/* Make sure the types really have the right sizes */
#define FETION_COMPILE_TIME_ASSERT(name, x)               \
	typedef int FETION_dummy_ ## name[(x) * 2 - 1]

FETION_COMPILE_TIME_ASSERT(uint8, sizeof(Uint8) == 1);
FETION_COMPILE_TIME_ASSERT(sint8, sizeof(Sint8) == 1);
FETION_COMPILE_TIME_ASSERT(uint16, sizeof(Uint16) == 2);
FETION_COMPILE_TIME_ASSERT(sint16, sizeof(Sint16) == 2);
FETION_COMPILE_TIME_ASSERT(uint32, sizeof(Uint32) == 4);
FETION_COMPILE_TIME_ASSERT(sint32, sizeof(Sint32) == 4);
FETION_COMPILE_TIME_ASSERT(uint64, sizeof(Uint64) == 8);
FETION_COMPILE_TIME_ASSERT(sint64, sizeof(Sint64) == 8);

#undef FETION_COMPILE_TIME_ASSERT

/** @} end of basic_types */

/**
* \defgroup endian_info Endianness information
* @{
*/

/**
* \def FETION_LIL_ENDIAN
* \brief Little endianness.
*/
#define FETION_LIL_ENDIAN  1234
/**
* \def FETION_BIG_ENDIAN
* \brief Big endianness.
*/
#define FETION_BIG_ENDIAN  4321

/* Pardon the mess, I'm trying to determine the endianness of this host.
*    I'm doing it by preprocessor defines rather than some sort of configure
*    script so that application code can use this too.  The "right" way would
*    be to dynamically generate this file on install, but that's a lot of work.
*/

/**
* \def FETION_BYTEORDER
* \brief The byte order (endianness) of the target system.
*
* This macro will be either defined to FETION_LIL_ENDIAN or FETION_BIG_ENDIAN.
* You can use the code like below
*
* \code
* #if FETION_BYTEORDER == FETION_LIL_ENDIAN
*     ... // code for little endian system.
* #else
*     ... // code for big endian system.
* #endif
* \endcode
*
* to write endianness independent code.
*/
#if  defined(__i386__) || defined(__ia64__) || \
	(defined(__alpha__) || defined(__alpha)) || \
	defined(__arm__) || \
	(defined(__CC_ARM) && !defined(__BIG_ENDIAN)) || \
	(defined(__mips__) && defined(__MIPSEL__)) || \
	defined(__LITTLE_ENDIAN__) || \
	defined(WIN32)
#define FETION_BYTEORDER   FETION_LIL_ENDIAN
#else
#define FETION_BYTEORDER   FETION_BIG_ENDIAN
#endif

/** @} end of endian_info */





/**
* \defgroup simple_types Simple and common types and macros
* @{
*/

#if !defined(__NODLL__) && defined (WIN32) 
#if defined(__LIBFETION_LIB__)
#define FX_EXPORT       __declspec(dllexport)
#else
//fixme: in windows we usual usr static library.
//#define FX_EXPORT       __declspec(dllimport) 
#define FX_EXPORT
#endif //defined(__LIBFETION_LIB__)
#else
#define FX_EXPORT
#endif //!defined(__NODLL__) && (defined (WIN32) || defined (__NUCLEUS_MNT__))


/**
* \var typedef int BOOL
* \brief A type definition for boolean value.
*/
#ifndef _HAVE_TYPE_BOOL
#ifndef BOOL
typedef int BOOL;
#endif
#endif

/**
* \def FALSE
* \brief FALSE value, defined as 0
*/
#ifndef FALSE
#define FALSE   (0)
#endif

/**
* \def TRUE
* \brief TRUE value, defined as 1
*/
#ifndef TRUE
#define TRUE    (1)
#endif

/**
* \def NULL
* \brief A value indicates null pointer.
*/
#ifndef NULL
#ifdef __cplusplus
#define NULL        (0L)
#else /* !__cplusplus */
#define NULL        ((void*) 0)
#endif /* !__cplusplus */
#endif

/** @} end of simple_types */


/**
* \defgroup win32_types Win32-like data types and macros
* @{
*/

/**
* \var typedef unsigned char BYTE
* \brief A type definition for unsigned character (byte).
*/
#ifndef _HAVE_TYPE_BYTE
typedef unsigned char   BYTE;
#endif

/**
* \var typedef signed char SBYTE
* \brief A type definition for signed character.
*/
typedef signed char     SBYTE;

/**
* \var typedef unsigned short WORD 
* \brief A type definition for unsigned short integer (word).
*/
#ifndef _HAVE_TYPE_WORD
typedef unsigned short  WORD;
#endif

/**
* \var typedef signed short SWORD 
* \brief A type definition for signed short integer.
*/
typedef signed short    SWORD;

/**
* \var typedef unsigned long DWORD
* \brief A type definition for unsigned long integer (double word).
*/
#ifndef _HAVE_TYPE_DWORD
typedef unsigned long   DWORD;
#endif

/**
* \var typedef signed long SDWORD
* \brief A type definition for signed long integer.
*/
typedef signed long     SDWORD;

/**
* \var typedef unsigned int UINT
* \brief A type definition for unsigned integer.
*/
#ifndef _HAVE_TYPE_UINT
typedef unsigned int    UINT;
#endif

/**
* \var typedef long LONG
* \brief A type definition for long integer.
*/
#ifndef _HAVE_TYPE_LONG
typedef long            LONG;
#endif

/**
* \var typedef unsigned int WPARAM
* \brief A type definition for the first message paramter.
*/
#if defined(_64BIT_)
typedef unsigned long    WPARAM;
#else
typedef unsigned int    WPARAM;
#endif


/**
* \var typedef unsigned long LPARAM
* \brief A type definition for the second message paramter.
*/
#ifdef WIN32 
#if _MSC_VER >= 1300 
typedef __w64 long LPARAM;
#else
typedef long   LPARAM;
#endif
#else  //no win32
typedef unsigned long   LPARAM;
#endif

#ifdef WIN32
#else
/**
* \def LOBYTE(w)
* \brief Returns the low byte of the word \a w.
*
* \sa MAKEWORD
*/
#define LOBYTE(w)           ((BYTE)(w))

/**
* \def HIBYTE(w)
* \brief Returns the high byte of the word \a w.
*
* \sa MAKEWORD
*/
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

/**
* \def MAKEWORD(low, high)
* \brief Makes a word from \a low byte and \a high byte.
*/
#define MAKEWORD(low, high) ((WORD)(((BYTE)((low) & 0xff )) | (((WORD)((BYTE) ((high) & 0xff ))) << 8)))

/**
* \def LOWORD(l)
* \brief Returns the low word of the double word \a l
*
* \sa MAKELONG
*/
#define LOWORD(l)           ((WORD)(DWORD)(l))

/**
* \def HIWORD(l)
* \brief Returns the high word of the double word \a l
*
* \sa MAKELONG
*/
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xFFFF))

/**
* \def LOSWORD(l)
* \brief Returns the low signed word of the double word \a l
*
* \sa MAKELONG
*/
#define LOSWORD(l)          ((SWORD)(DWORD)(l))

/**
* \def HISWORD(l)
* \brief Returns the high signed word of the double word \a l
*
* \sa MAKELONG
*/
#define HISWORD(l)          ((SWORD)((((DWORD)(l)) >> 16) & 0xFFFF))

/**
* \def MAKELONG(low, high)
* \brief Makes a double word from \a low word and \a high word.
*/
#define MAKELONG(low, high) ((DWORD)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))

#endif  //#ifdef WIN32

/** @} end of win32_types */



/**
* \defgroup misc_macros Miscellaneous macros
* @{
*/

/**
* \def TABLESIZE(table)
* \brief A macro returns the number of elements in a \a table.
*/
#define TABLESIZE(table)    (sizeof(table)/sizeof(table[0]))

/* MAX/MIN/ABS macors */
/**
* \def MAX(x, y)
* \brief A macro returns the maximum of \a x and \a y.
*/
#ifndef MAX
#define MAX(x, y)           (((x) > (y))?(x):(y))
#endif
/**
* \def MIN(x, y)
* \brief A macro returns the minimum of \a x and \a y.
*/
#ifndef MIN
#define MIN(x, y)           (((x) < (y))?(x):(y))
#endif
/**
* \def ABS(x)
* \brief A macro returns the absolute value of \a x.
*/
#ifndef ABS
#define ABS(x)              (((x)<0) ? -(x) : (x))
#endif

/**
* \def CLAMP(x, low, high)
* \brief A macro returns the middle value  of \a x, \a low and \a high.
*/
#ifndef CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#endif

/**
* \def ISSPACE(c)
* \brief A macro returns the absolute value of \a c.
*/
#define ISSPACE(c)		((c) == ' ' || (c) == '\f' || (c) == '\n' || \
	(c) == '\r' || (c) == '\t' || (c) == '\v')

/**
* \def ISUPPER(c)
* \brief A macro judge the value of \a c is a upper character.
*/
#define ISUPPER(c)		((c) >= 'A' && (c) <= 'Z')

/**
* \def ISLOWER(c)
* \brief A macro judge the value of \a c is a lower character.
*/
#define ISLOWER(c)		((c) >= 'a' && (c) <= 'z')

/**
* \def ISALPHA(c)
* \brief A macro judge the value of \a c is a character.
*/
#define ISALPHA(c)		(ISUPPER (c) || ISLOWER (c))

/**
* \def TOUPPER(c)
* \brief A macro returns the is a upper character. of value \a c.
*/
#define	TOUPPER(c)		(ISLOWER (c) ? (c) - 'a' + 'A' : (c))

/**
* \def TOLOWER(c)
* \brief A macro returns the is a lower character. of value \a c.
*/
#define	TOLOWER(c)		(ISUPPER (c) ? (c) - 'A' + 'a' : (c))

/** @} end of misc_macros */

/** @} end of macros_types */


#ifndef HAVE_TIME
typedef unsigned int time_t;

struct tm {
	int    tm_sec;   /* seconds [0,61] */
	int    tm_min;   /* minutes [0,59] */
	int    tm_hour;  /* hour [0,23] */
	int    tm_mday;  /* day of month [1,31] */
	int    tm_mon;   /* month of year [0,11] */
	int    tm_year;  /* years since 1900 */
	int    tm_wday;  /* day of week [0,6] (Sunday = 0) */
	int    tm_yday;  /* day of year [0,365] */
	int    tm_isdst; /* daylight savings flag */
};
#endif

/**
* \defgroup cb_func Event CallBack Function
* @{
*/

/**
* \var typedef void (*EventListener) (int message, WPARAM wParam, LPARAM lParam, void* args)
* brief the callback function of fetion event.
*
*
* \param message The fetion event type.
* \sa events
*/

typedef void (*EventListener) (int message, WPARAM wParam, LPARAM lParam, void* args);

/** @} end of cb_func */


#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif  //_LIBFETION_COMMON_H


