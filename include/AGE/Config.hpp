/**
 * Defines the AGE engine version numbers and provides OS identification and
 * other OS and OS independent values for all AGE engines.
 *
 * @file include/AGE/Config.hpp
 * @author Ryan Lindeman
 * @date 20110127 - Initial Release
 * @date 20110131 - Added define documentation
 * @date 20110218 - Next minor release of AGE
 * @date 20110318 - Next minor release of AGE
 * @date 20110611 - Next minor release of AGE
 * @date 20110618 - Next minor release of AGE
 * @date 20110627 - Next minor release of AGE
 * @date 20110831 - Next minor release of AGE
 * @date 20110906 - Remove use of AGE_STRICMP
 * @date 20120211 - Next minor release of AGE
 * @date 20120211 - Next minor release of AGE
 * @date 20120330 - Next minor release of AGE
 */
#ifndef AGE_CONFIG_HPP
#define AGE_CONFIG_HPP


////////////////////////////////////////////////////////////
// Define the AGE version
////////////////////////////////////////////////////////////
/// Define the AGE Major version number
#define AGE_VERSION_MAJOR 0
/// Define the AGE Minor version number
#define AGE_VERSION_MINOR 22


////////////////////////////////////////////////////////////
// Identify the operating system
////////////////////////////////////////////////////////////
#if defined(_WIN32) || defined(__WIN32__)

/// Define a AGE Windows OS flag
#define AGE_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(linux) || defined(__linux)

/// Define a AGE Linux OS flag
#define AGE_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

/// Define a AGE Mac OS flag
#define AGE_MACOS

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

/// Define a AGE FreeBSD OS flag
#define AGE_FREEBSD

#else

// Unsupported system
#error This operating system is not supported by AGE library

#endif


////////////////////////////////////////////////////////////
// Identify the endianess
////////////////////////////////////////////////////////////
#if defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
  defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__) || defined(__hppa__)

/// Define a AGE Big endian flag
#define AGE_ENDIAN_BIG

#else

/// Define a AGE Little endian flag
#define AGE_ENDIAN_LITTLE

#endif


////////////////////////////////////////////////////////////
// Define a portable debug macro
////////////////////////////////////////////////////////////
#if !defined(NDEBUG)

/// Define a AGE Debug build flag
#define AGE_DEBUG

#endif


////////////////////////////////////////////////////////////
// Define portable import / export macros
////////////////////////////////////////////////////////////
#if defined(AGE_WINDOWS) && !defined(AGE_STATIC)

#ifdef AGE_EXPORTS

// From DLL side, we must export
/// Define the AGE DLL API for exporting classes and methods
#define AGE_API __declspec(dllexport)

#else

// From client application side, we must import
/// Define the AGE DLL API for importing classes and methods
#define AGE_API __declspec(dllimport)

#endif

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning.
// You can read lots ot different things about it, but the point is the code will
// just work fine, and so the simplest way to get rid of this warning is to disable it
#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else

// Other platforms and static build don't need these export macros
/// Define the AGE DLL API for non DLL OS platforms as nothing
#define AGE_API

#endif


////////////////////////////////////////////////////////////
// Define portable fixed-size types
////////////////////////////////////////////////////////////
#include <stdint.h>
namespace AGE
{
  // All "common" platforms use the same size for char, short and int
  // (basically there are 3 types for 3 sizes, so no other match is possible),
  // we can use them without doing any kind of check

  // 8 bits integer types
  typedef signed   char Int8;     ///< Standard 8 bit signed integer
  typedef unsigned char Uint8;    ///< Standard 8 bit unsigned integer

  // 16 bits integer types
  typedef signed   short Int16;   ///< Standard 16 bit signed integer
  typedef unsigned short Uint16;  ///< Standard 16 bit unsigned integer

  // 32 bits integer types
  typedef signed   int Int32;     ///< Standard 32 bit signed integer
  typedef unsigned int Uint32;    ///< Standard 32 bit unsigned integer

  // 64 bits integer types
#if defined(_MSC_VER)
  typedef signed   __int64 Int64;  ///< Standard 64 bit signed integer
  typedef unsigned __int64 Uint64; ///< Standard 64 bit unsigned integer
#else
  typedef int64_t          Int64;  ///< Standard 64 bit signed integer
  typedef uint64_t         Uint64; ///< Standard 64 bit unsigned integer
#endif

} // namespace AGE


#endif // AGE_CONFIG_HPP
