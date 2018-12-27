/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* H5pubconf.h  Generated By CMake during the configuration */

#ifndef H5_CONFIG_H_
#define H5_CONFIG_H_

/* Define if the Windows virtual file driver should be compiled */
#define H5_HAVE_WINDOWS @H5_HAVE_WINDOWS@

/* Define if using MinGW */
////#define H5_HAVE_MINGW @H5_HAVE_MINGW@

/* Define if on the Windows platform and default WIN32 API */
//#define H5_HAVE_WIN32_API @H5_HAVE_WIN32_API@

/* Define if using a Windows compiler (i.e. Visual Studio) */
//#define H5_HAVE_VISUAL_STUDIO @H5_HAVE_VISUAL_STUDIO@

/* Define if building universal (internal helper macro) */
//#define H5_AC_APPLE_UNIVERSAL_BUILD @H5_AC_APPLE_UNIVERSAL_BUILD@

/* Define if C++ compiler recognizes offsetof */
//#define H5_CXX_HAVE_OFFSETOF @H5_CXX_HAVE_OFFSETOF@

/* Define the default plugins path to compile */
//#define H5_DEFAULT_PLUGINDIR "@H5_DEFAULT_PLUGINDIR@"

/* Define if `dev_t' is a scalar */
//#define H5_DEV_T_IS_SCALAR @H5_DEV_T_IS_SCALAR@

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
//#define H5_FC_DUMMY_MAIN @H5_FC_DUMMY_MAIN@

/* Define if F77 and FC dummy `main' functions are identical. */
//#define H5_FC_DUMMY_MAIN_EQ_F77 @H5_FC_DUMMY_MAIN_EQ_F77@

/* Define to a macro mangling the given C identifier (in lower and upper
   case), which must not contain underscores, for linking with Fortran. */
////#define @H5_FC_FUNC@

/* As FC_FUNC, but for C identifiers containing underscores. */
////#define @H5_FC_FUNC_@

/* Define if Fortran C_LONG_DOUBLE is different from C_DOUBLE */
////#define H5_FORTRAN_C_LONG_DOUBLE_IS_UNIQUE @H5_FORTRAN_C_LONG_DOUBLE_IS_UNIQUE@

/* Define if we have Fortran C_LONG_DOUBLE */
////#define H5_FORTRAN_HAVE_C_LONG_DOUBLE @H5_FORTRAN_HAVE_C_LONG_DOUBLE@

/* Define if we have Fortran intrinsic C_SIZEOF */
////#define H5_FORTRAN_HAVE_C_SIZEOF @H5_FORTRAN_HAVE_C_SIZEOF@

/* Define if we have Fortran intrinsic SIZEOF */
////#define H5_FORTRAN_HAVE_SIZEOF @H5_FORTRAN_HAVE_SIZEOF@

/* Define if we have Fortran intrinsic STORAGE_SIZE */
////#define H5_FORTRAN_HAVE_STORAGE_SIZE @H5_FORTRAN_HAVE_STORAGE_SIZE@

/* Determine the size of C long double */
////#define H5_FORTRAN_SIZEOF_LONG_DOUBLE @H5_FORTRAN_SIZEOF_LONG_DOUBLE@

/* Define Fortran compiler ID */
////#define H5_Fortran_COMPILER_ID @H5_Fortran_COMPILER_ID@

/* Define valid Fortran INTEGER KINDs */
////#define H5_H5CONFIG_F_IKIND @H5_HH5_H5CONFIG_F_NUM_RKIND5CONFIG_F_IKIND@

/* Define number of valid Fortran INTEGER KINDs */
////#define H5_H5CONFIG_F_NUM_IKIND @H5_H5CONFIG_F_NUM_IKIND@

/* Define number of valid Fortran REAL KINDs */
////#define H5_H5CONFIG_F_NUM_RKIND @H5_H5CONFIG_F_NUM_RKIND@

/* Define valid Fortran REAL KINDs */
////#define H5_H5CONFIG_F_RKIND @H5_H5CONFIG_F_RKIND@

/* Define valid Fortran REAL KINDs Sizeof */
////#define H5_H5CONFIG_F_RKIND_SIZEOF @H5_H5CONFIG_F_RKIND_SIZEOF@

/* Define to 1 if you have the `alarm' function. */
////#define H5_HAVE_ALARM @H5_HAVE_ALARM@

/* Define to 1 if you have the `asprintf' function. */
////#define H5_HAVE_ASPRINTF @H5_HAVE_ASPRINTF@

/* Define if the __attribute__(()) extension is present */
////#define H5_HAVE_ATTRIBUTE @H5_HAVE_ATTRIBUTE@

/* Define if the compiler understands C99 designated initialization of structs
   and unions */
////#define H5_HAVE_C99_DESIGNATED_INITIALIZER @H5_HAVE_C99_DESIGNATED_INITIALIZER@

/* Define if the compiler understands the __func__ keyword */
////#define H5_HAVE_C99_FUNC @H5_HAVE_C99_FUNC@

/* Define to 1 if you have the `clock_gettime' function. */
////#define H5_HAVE_CLOCK_GETTIME @H5_HAVE_CLOCK_GETTIME@

/* Define if the function stack tracing code is to be compiled in */
////#define H5_HAVE_CODESTACK @H5_HAVE_CODESTACK@

/* Define if Darwin or Mac OS X */
////#define H5_HAVE_DARWIN @H5_HAVE_DARWIN@

/* Define to 1 if you have the `difftime' function. */
////#define H5_HAVE_DIFFTIME @H5_HAVE_DIFFTIME@

/* Define if the direct I/O virtual file driver (VFD) should be compiled */
////#define H5_HAVE_DIRECT @H5_HAVE_DIRECT@

/* Define to 1 if you have the <dirent.h> header file. */
////#define H5_HAVE_DIRENT_H @H5_HAVE_DIRENT_H@

/* Define to 1 if you have the <dlfcn.h> header file. */
////#define H5_HAVE_DLFCN_H @H5_HAVE_DLFCN_H@

/* Define to 1 if you have the <dmalloc.h> header file. */
////#define H5_HAVE_DMALLOC_H @H5_HAVE_DMALLOC_H@

/* Define if library information should be embedded in the executables */
////#define H5_HAVE_EMBEDDED_LIBINFO @H5_HAVE_EMBEDDED_LIBINFO@

/* Define to 1 if you have the `fcntl' function. */
////#define H5_HAVE_FCNTL @H5_HAVE_FCNTL@

/* Define to 1 if you have the <features.h> header file. */
////#define H5_HAVE_FEATURES_H @H5_HAVE_FEATURES_H@

/* Define if support for deflate (zlib) filter is enabled */
////#define H5_HAVE_FILTER_DEFLATE @H5_HAVE_FILTER_DEFLATE@

/* Define if support for szip filter is enabled */
////#define H5_HAVE_FILTER_SZIP @H5_HAVE_FILTER_SZIP@

/* Determine if __float128 is available */
////#define H5_HAVE_FLOAT128 @H5_HAVE_FLOAT128@

/* Define to 1 if you have the `flock' function. */
////#define H5_HAVE_FLOCK @H5_HAVE_FLOCK@

/* Define to 1 if you have the `fork' function. */
////#define H5_HAVE_FORK @H5_HAVE_FORK@

/* Define to 1 if you have the `frexpf' function. */
////#define H5_HAVE_FREXPF @H5_HAVE_FREXPF@

/* Define to 1 if you have the `frexpl' function. */
////#define H5_HAVE_FREXPL @H5_HAVE_FREXPL@

/* Define to 1 if you have the `fseeko' function. */
////#define H5_HAVE_FSEEKO @H5_HAVE_FSEEKO@

/* Define to 1 if you have the `fseeko64' function. */
////#define H5_HAVE_FSEEKO64 @H5_HAVE_FSEEKO64@

/* Define to 1 if you have the `fstat64' function. */
////#define H5_HAVE_FSTAT64 @H5_HAVE_FSTAT64@

/* Define to 1 if you have the `ftello' function. */
////#define H5_HAVE_FTELLO @H5_HAVE_FTELLO@

/* Define to 1 if you have the `ftello64' function. */
////#define H5_HAVE_FTELLO64 @H5_HAVE_FTELLO64@

/* Define to 1 if you have the `ftruncate64' function. */
////#define H5_HAVE_FTRUNCATE64 @H5_HAVE_FTRUNCATE64@

/* Define if the compiler understands the __FUNCTION__ keyword */
////#define H5_HAVE_FUNCTION @H5_HAVE_FUNCTION@

/* Define to 1 if you have the `GetConsoleScreenBufferInfo' function. */
////#define H5_HAVE_GETCONSOLESCREENBUFFERINFO @H5_HAVE_GETCONSOLESCREENBUFFERINFO@

/* Define to 1 if you have the `gethostname' function. */
////#define H5_HAVE_GETHOSTNAME @H5_HAVE_GETHOSTNAME@

/* Define to 1 if you have the `getpwuid' function. */
////#define H5_HAVE_GETPWUID @H5_HAVE_GETPWUID@

/* Define to 1 if you have the `getrusage' function. */
////#define H5_HAVE_GETRUSAGE @H5_HAVE_GETRUSAGE@

/* Define to 1 if you have the `gettextinfo' function. */
////#define H5_HAVE_GETTEXTINFO @H5_HAVE_GETTEXTINFO@

/* Define to 1 if you have the `gettimeofday' function. */
////#define H5_HAVE_GETTIMEOFDAY @H5_HAVE_GETTIMEOFDAY@

/* Define if the compiler understands inline */
////#define H5_HAVE_INLINE @H5_HAVE_INLINE@

/* Define if parallel library will contain instrumentation to detect correct
   optimization operation */
//#define H5_HAVE_INSTRUMENTED_LIBRARY @H5_HAVE_INSTRUMENTED_LIBRARY@

/* Define to 1 if you have the <inttypes.h> header file. */
//#define H5_HAVE_INTTYPES_H @H5_HAVE_INTTYPES_H@

/* Define to 1 if you have the `ioctl' function. */
//#define H5_HAVE_IOCTL @H5_HAVE_IOCTL@

/* Define to 1 if you have the <io.h> header file. */
//#define H5_HAVE_IO_H @H5_HAVE_IO_H@

/* Define to 1 if you have the `dl' library (-ldl). */
//#define H5_HAVE_LIBDL @H5_HAVE_LIBDL@

/* Define to 1 if you have the `dmalloc' library (-ldmalloc). */
//#define H5_HAVE_LIBDMALLOC @H5_HAVE_LIBDMALLOC@

/* Define to 1 if you have the `m' library (-lm). */
//#define H5_HAVE_LIBM @H5_HAVE_LIBM@

/* Define to 1 if you have the `mpe' library (-lmpe). */
//#define H5_HAVE_LIBMPE @H5_HAVE_LIBMPE@

/* Define to 1 if you have the `pthread' library (-lpthread). */
//#define H5_HAVE_LIBPTHREAD @H5_HAVE_LIBPTHREAD@

/* Define to 1 if you have the `sz' library (-lsz). */
//#define H5_HAVE_LIBSZ @H5_HAVE_LIBSZ@

/* Define to 1 if you have the `ws2_32' library (-lws2_32). */
//#define H5_HAVE_LIBWS2_32 @H5_HAVE_LIBWS2_32@

/* Define to 1 if you have the `z' library (-lz). */
//#define H5_HAVE_LIBZ @H5_HAVE_LIBZ@

/* Define to 1 if you have the `llround' function. */
//#define H5_HAVE_LLROUND @H5_HAVE_LLROUND@

/* Define to 1 if you have the `llroundf' function. */
//#define H5_HAVE_LLROUNDF @H5_HAVE_LLROUNDF@

/* Define to 1 if you have the `longjmp' function. */
//#define H5_HAVE_LONGJMP @H5_HAVE_LONGJMP@

/* Define to 1 if you have the `lround' function. */
//#define H5_HAVE_LROUND @H5_HAVE_LROUND@

/* Define to 1 if you have the `lroundf' function. */
//#define H5_HAVE_LROUNDF @H5_HAVE_LROUNDF@

/* Define to 1 if you have the `lseek64' function. */
//#define H5_HAVE_LSEEK64 @H5_HAVE_LSEEK64@

/* Define to 1 if you have the `lstat' function. */
//#define H5_HAVE_LSTAT @H5_HAVE_LSTAT@

/* Define to 1 if you have the <mach/mach_time.h> header file. */
//#define H5_HAVE_MACH_MACH_TIME_H @H5_HAVE_MACH_MACH_TIME_H@

/* Define to 1 if you have the <memory.h> header file. */
//#define H5_HAVE_MEMORY_H @H5_HAVE_MEMORY_H@

/* Define if we have MPE support */
//#define H5_HAVE_MPE @H5_HAVE_MPE@

/* Define to 1 if you have the <mpe.h> header file. */
//#define H5_HAVE_MPE_H @H5_HAVE_MPE_H@

/* Define if `MPI_Comm_c2f' and `MPI_Comm_f2c' exists */
//#define H5_HAVE_MPI_MULTI_LANG_Comm @H5_HAVE_MPI_MULTI_LANG_Comm@

/* Define if `MPI_Info_c2f' and `MPI_Info_f2c' exists */
//#define H5_HAVE_MPI_MULTI_LANG_Info @H5_HAVE_MPI_MULTI_LANG_Info@

/* Define if we have parallel support */
//#define H5_HAVE_PARALLEL @H5_HAVE_PARALLEL@

/* Define to 1 if you have the <pthread.h> header file. */
//#define H5_HAVE_PTHREAD_H @H5_HAVE_PTHREAD_H@

/* Define to 1 if you have the <quadmath.h> header file. */
//#define H5_HAVE_QUADMATH_H @H5_HAVE_QUADMATH_H@

/* Define to 1 if you have the `random' function. */
//#define H5_HAVE_RANDOM @H5_HAVE_RANDOM@

/* Define to 1 if you have the `rand_r' function. */
//#define H5_HAVE_RAND_R @H5_HAVE_RAND_R@

/* Define to 1 if you have the `round' function. */
//#define H5_HAVE_ROUND @H5_HAVE_ROUND@

/* Define to 1 if you have the `roundf' function. */
//#define H5_HAVE_ROUNDF @H5_HAVE_ROUNDF@

/* Define to 1 if you have the `setjmp' function. */
//#define H5_HAVE_SETJMP @H5_HAVE_SETJMP@

/* Define to 1 if you have the <setjmp.h> header file. */
//#define H5_HAVE_SETJMP_H @H5_HAVE_SETJMP_H@

/* Define to 1 if you have the `setsysinfo' function. */
//#define H5_HAVE_SETSYSINFO @H5_HAVE_SETSYSINFO@

/* Define to 1 if you have the `siglongjmp' function. */
//#define H5_HAVE_SIGLONGJMP @H5_HAVE_SIGLONGJMP@

/* Define to 1 if you have the `signal' function. */
//#define H5_HAVE_SIGNAL @H5_HAVE_SIGNAL@

/* Define to 1 if you have the `sigprocmask' function. */
//#define H5_HAVE_SIGPROCMASK @H5_HAVE_SIGPROCMASK@

/* Define to 1 if you have the `sigsetjmp' function. */
//#define H5_HAVE_SIGSETJMP @H5_HAVE_SIGSETJMP@

/* Define to 1 if you have the `snprintf' function. */
//#define H5_HAVE_SNPRINTF @H5_HAVE_SNPRINTF@

/* Define to 1 if you have the `srandom' function. */
//#define H5_HAVE_SRANDOM @H5_HAVE_SRANDOM@

/* Define to 1 if you have the `stat64' function. */
//#define H5_HAVE_STAT64 @H5_HAVE_STAT64@

/* Define if `struct stat' has the `st_blocks' field */
//#define H5_HAVE_STAT_ST_BLOCKS @H5_HAVE_STAT_ST_BLOCKS@

/* Define to 1 if you have the <stdbool.h> header file. */
//#define H5_HAVE_STDBOOL_H @H5_HAVE_STDBOOL_H@

/* Define to 1 if you have the <stddef.h> header file. */
//#define H5_HAVE_STDDEF_H @H5_HAVE_STDDEF_H@

/* Define to 1 if you have the <stdint.h> header file. */
//#define H5_HAVE_STDINT_H @H5_HAVE_STDINT_H@

/* Define to 1 if you have the <stdint.h> header file for Cplusplus. */
//#define H5_HAVE_STDINT_H_CXX @H5_HAVE_STDINT_H_CXX@

/* Define to 1 if you have the <stdlib.h> header file. */
//#define H5_HAVE_STDLIB_H @H5_HAVE_STDLIB_H@

/* Define to 1 if you have the `strdup' function. */
//#define H5_HAVE_STRDUP @H5_HAVE_STRDUP@

/* Define to 1 if you have the `strtoll' function. */
//#define H5_HAVE_STRTOLL @H5_HAVE_STRTOLL@

/* Define to 1 if you have the `strtoull' function. */
//#define H5_HAVE_STRTOULL @H5_HAVE_STRTOULL@

/* Define to 1 if you have the <strings.h> header file. */
//#define H5_HAVE_STRINGS_H @H5_HAVE_STRINGS_H@

/* Define to 1 if you have the <string.h> header file. */
//#define H5_HAVE_STRING_H @H5_HAVE_STRING_H@

/* Define if `struct text_info' is defined */
//#define H5_HAVE_STRUCT_TEXT_INFO @H5_HAVE_STRUCT_TEXT_INFO@

/* Define if `struct videoconfig' is defined */
//#define H5_HAVE_STRUCT_VIDEOCONFIG @H5_HAVE_STRUCT_VIDEOCONFIG@

/* Define to 1 if you have the `symlink' function. */
//#define H5_HAVE_SYMLINK @H5_HAVE_SYMLINK@

/* Define to 1 if you have the `system' function. */
//#define H5_HAVE_SYSTEM @H5_HAVE_SYSTEM@

/* Define to 1 if you have the <sys/file.h> header file. */
//#define H5_HAVE_SYS_FILE_H @H5_HAVE_SYS_FILE_H@

/* Define to 1 if you have the <sys/ioctl.h> header file. */
//#define H5_HAVE_SYS_IOCTL_H @H5_HAVE_SYS_IOCTL_H@

/* Define to 1 if you have the <sys/resource.h> header file. */
//#define H5_HAVE_SYS_RESOURCE_H @H5_HAVE_SYS_RESOURCE_H@

/* Define to 1 if you have the <sys/socket.h> header file. */
//#define H5_HAVE_SYS_SOCKET_H @H5_HAVE_SYS_SOCKET_H@

/* Define to 1 if you have the <sys/stat.h> header file. */
//#define H5_HAVE_SYS_STAT_H @H5_HAVE_SYS_STAT_H@

/* Define to 1 if you have the <sys/timeb.h> header file. */
//#define H5_HAVE_SYS_TIMEB_H @H5_HAVE_SYS_TIMEB_H@

/* Define to 1 if you have the <sys/time.h> header file. */
//#define H5_HAVE_SYS_TIME_H @H5_HAVE_SYS_TIME_H@

/* Define to 1 if you have the <sys/types.h> header file. */
//#define H5_HAVE_SYS_TYPES_H @H5_HAVE_SYS_TYPES_H@

/* Define to 1 if you have the <szlib.h> header file. */
//#define H5_HAVE_SZLIB_H @H5_HAVE_SZLIB_H@

/* Define if we have thread safe support */
//#define H5_HAVE_THREADSAFE @H5_HAVE_THREADSAFE@

/* Define if `timezone' is a global variable */
//#define H5_HAVE_TIMEZONE @H5_HAVE_TIMEZONE@

/* Define if the ioctl TIOCGETD is defined */
//#define H5_HAVE_TIOCGETD @H5_HAVE_TIOCGETD@

/* Define if the ioctl TIOGWINSZ is defined */
//#define H5_HAVE_TIOCGWINSZ @H5_HAVE_TIOCGWINSZ@

/* Define to 1 if you have the `tmpfile' function. */
//#define H5_HAVE_TMPFILE @H5_HAVE_TMPFILE@

/* Define if `tm_gmtoff' is a member of `struct tm' */
//#define H5_HAVE_TM_GMTOFF @H5_HAVE_TM_GMTOFF@

/* Define to 1 if you have the <unistd.h> header file. */
//#define H5_HAVE_UNISTD_H @H5_HAVE_UNISTD_H@

/* Define to 1 if you have the `vasprintf' function. */
//#define H5_HAVE_VASPRINTF @H5_HAVE_VASPRINTF@

/* Define to 1 if you have the `vsnprintf' function. */
//#define H5_HAVE_VSNPRINTF @H5_HAVE_VSNPRINTF@

/* Define to 1 if you have the `waitpid' function. */
//#define H5_HAVE_WAITPID @H5_HAVE_WAITPID@

/* Define to 1 if you have the 'InitOnceExecuteOnce' function. */
//#define H5_HAVE_WIN_THREADS @H5_HAVE_WIN_THREADS@

/* Define if your system has window style path name. */
//#define H5_HAVE_WINDOW_PATH @H5_HAVE_WINDOW_PATH@

/* Define to 1 if you have the <winsock.h> header file. */
//#define H5_HAVE_WINSOCK2_H @H5_HAVE_WINSOCK2_H@

/* Define to 1 if you have the <zlib.h> header file. */
//#define H5_HAVE_ZLIB_H @H5_HAVE_ZLIB_H@

/* Define to 1 if you have the `_getvideoconfig' function. */
//#define H5_HAVE__GETVIDEOCONFIG @H5_HAVE__GETVIDEOCONFIG@

/* Define to 1 if you have the `_scrsize' function. */
//#define H5_HAVE__SCRSIZE @H5_HAVE__SCRSIZE@

/* Define if the compiler understands __inline */
//#define H5_HAVE___INLINE @H5_HAVE___INLINE@

/* Define if the compiler understands __inline__ */
//#define H5_HAVE___INLINE__ @H5_HAVE___INLINE__@

/* Define if HDF5's high-level library headers should be included in hdf5.h */
//#define H5_INCLUDE_HL @H5_INCLUDE_HL@

/* Define if your system can convert long double to (unsigned) long long
   values correctly. */
//#define H5_LDOUBLE_TO_LLONG_ACCURATE @H5_LDOUBLE_TO_LLONG_ACCURATE@

/* Define if your system converts long double to (unsigned) long values with
   special algorithm. */
//#define H5_LDOUBLE_TO_LONG_SPECIAL @H5_LDOUBLE_TO_LONG_SPECIAL@

/* Define if your system can convert (unsigned) long long to long double
   values correctly. */
//#define H5_LLONG_TO_LDOUBLE_CORRECT @H5_LLONG_TO_LDOUBLE_CORRECT@

/* Define if your system can convert (unsigned) long to long double values
   with special algorithm. */
//#define H5_LONG_TO_LDOUBLE_SPECIAL @H5_LONG_TO_LDOUBLE_SPECIAL@

/* Define to the sub-directory where libtool stores uninstalled libraries. */
//#define H5_LT_OBJDIR @H5_LT_OBJDIR@

/* Define to enable internal memory allocation sanity checking. */
/* //#define H5_MEMORY_ALLOC_SANITY_CHECK @H5_MEMORY_ALLOC_SANITY_CHECK@ ** Define in CMakeLists.txt */

/* Define if the metadata trace file code is to be compiled in */
//#define H5_METADATA_TRACE_FILE @H5_METADATA_TRACE_FILE@

/* Define if we can violate pointer alignment restrictions */
//#define H5_NO_ALIGNMENT_RESTRICTIONS @H5_NO_ALIGNMENT_RESTRICTIONS@

/* Define if deprecated public API symbols are disabled */
//#define H5_NO_DEPRECATED_SYMBOLS @H5_NO_DEPRECATED_SYMBOLS@

/* Name of package */
//#define H5_PACKAGE "@HDF5_PACKAGE@"

/* Define to the address where bug reports for this package should be sent. */
//#define H5_PACKAGE_BUGREPORT "@HDF5_PACKAGE_BUGREPORT@"

/* Define to the full name of this package. */
//#define H5_PACKAGE_NAME "@HDF5_PACKAGE_NAME@"

/* Define to the full name and version of this package. */
//#define H5_PACKAGE_STRING "@HDF5_PACKAGE_STRING@"

/* Define to the one symbol short name of this package. */
//#define H5_PACKAGE_TARNAME "@HDF5_PACKAGE_TARNAME@"

/* Define to the home page for this package. */
//#define H5_PACKAGE_URL "@HDF5_PACKAGE_URL@"

/* Define to the version of this package. */
//#define H5_PACKAGE_VERSION "@HDF5_PACKAGE_VERSION_STRING@"

/* Determine the maximum decimal precision in C */
//#define H5_PAC_C_MAX_REAL_PRECISION @H5_PAC_C_MAX_REAL_PRECISION@

/* Define Fortran Maximum Real Decimal Precision */
//#define H5_PAC_FC_MAX_REAL_PRECISION @H5_PAC_FC_MAX_REAL_PRECISION@

/* Width for printf() for type `long long' or `__int64', use `ll' */
//#define H5_PRINTF_LL_WIDTH @H5_PRINTF_LL_WIDTH@

/* The size of `bool', as computed by sizeof. */
//#define H5_SIZEOF_BOOL @H5_SIZEOF_BOOL@

/* The size of `char', as computed by sizeof. */
//#define H5_SIZEOF_CHAR @H5_SIZEOF_CHAR@

/* The size of `double', as computed by sizeof. */
//#define H5_SIZEOF_DOUBLE @H5_SIZEOF_DOUBLE@

/* The size of `float', as computed by sizeof. */
//#define H5_SIZEOF_FLOAT @H5_SIZEOF_FLOAT@

/* The size of `int', as computed by sizeof. */
//#define H5_SIZEOF_INT @H5_SIZEOF_INT@

/* The size of `int16_t', as computed by sizeof. */
//#define H5_SIZEOF_INT16_T @H5_SIZEOF_INT16_T@

/* The size of `int32_t', as computed by sizeof. */
//#define H5_SIZEOF_INT32_T @H5_SIZEOF_INT32_T@

/* The size of `int64_t', as computed by sizeof. */
//#define H5_SIZEOF_INT64_T @H5_SIZEOF_INT64_T@

/* The size of `int8_t', as computed by sizeof. */
//#define H5_SIZEOF_INT8_T @H5_SIZEOF_INT8_T@

/* The size of `int_fast16_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_FAST16_T @H5_SIZEOF_INT_FAST16_T@

/* The size of `int_fast32_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_FAST32_T @H5_SIZEOF_INT_FAST32_T@

/* The size of `int_fast64_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_FAST64_T @H5_SIZEOF_INT_FAST64_T@

/* The size of `int_fast8_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_FAST8_T @H5_SIZEOF_INT_FAST8_T@

/* The size of `int_least16_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_LEAST16_T @H5_SIZEOF_INT_LEAST16_T@

/* The size of `int_least32_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_LEAST32_T @H5_SIZEOF_INT_LEAST32_T@

/* The size of `int_least64_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_LEAST64_T @H5_SIZEOF_INT_LEAST64_T@

/* The size of `int_least8_t', as computed by sizeof. */
//#define H5_SIZEOF_INT_LEAST8_T @H5_SIZEOF_INT_LEAST8_T@

#if !defined(__APPLE__)
/* The size of `size_t', as computed by sizeof. */
//#define H5_SIZEOF_SIZE_T @H5_SIZEOF_SIZE_T@

/* The size of `ssize_t', as computed by sizeof. */
//#define H5_SIZEOF_SSIZE_T @H5_SIZEOF_SSIZE_T@

/* The size of `long', as computed by sizeof. */
//#define H5_SIZEOF_LONG @H5_SIZEOF_LONG@

#else
   # if defined(__LP64__) && __LP64__
  //#define H5_SIZEOF_LONG 8
  //#define H5_SIZEOF_SIZE_T 8
  //#define H5_SIZEOF_SSIZE_T 8
  # else
  //#define H5_SIZEOF_LONG 4
  //#define H5_SIZEOF_SIZE_T 4
  //#define H5_SIZEOF_SSIZE_T 4
  # endif

#endif

/* The size of `long double', as computed by sizeof. */
//#define H5_SIZEOF_LONG_DOUBLE @H5_SIZEOF_LONG_DOUBLE@

/* Define size of long long and/or __int64 bit integer type only if the type
   exists.  */
//#if !defined(__APPLE__)
 //#define H5_SIZEOF_LONG_LONG @H5_SIZEOF_LONG_LONG@
//#else
 #define H5_SIZEOF_LONG_LONG 8
//#endif

/* The size of `off64_t', as computed by sizeof. */
//#define H5_SIZEOF_OFF64_T @H5_SIZEOF_OFF64_T@

/* The size of `off_t', as computed by sizeof. */
//#define H5_SIZEOF_OFF_T @H5_SIZEOF_OFF_T@

/* The size of `ptrdiff_t', as computed by sizeof. */
//#define H5_SIZEOF_PTRDIFF_T @H5_SIZEOF_PTRDIFF_T@

/* The size of `short', as computed by sizeof. */
//#define H5_SIZEOF_SHORT @H5_SIZEOF_SHORT@

/* The size of `uint16_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT16_T @H5_SIZEOF_UINT16_T@

/* The size of `uint32_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT32_T @H5_SIZEOF_UINT32_T@

/* The size of `uint64_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT64_T @H5_SIZEOF_UINT64_T@

/* The size of `uint8_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT8_T @H5_SIZEOF_UINT8_T@

/* The size of `uint_fast16_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_FAST16_T @H5_SIZEOF_UINT_FAST16_T@

/* The size of `uint_fast32_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_FAST32_T @H5_SIZEOF_UINT_FAST32_T@

/* The size of `uint_fast64_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_FAST64_T @H5_SIZEOF_UINT_FAST64_T@

/* The size of `uint_fast8_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_FAST8_T @H5_SIZEOF_UINT_FAST8_T@

/* The size of `uint_least16_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_LEAST16_T @H5_SIZEOF_UINT_LEAST16_T@

/* The size of `uint_least32_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_LEAST32_T @H5_SIZEOF_UINT_LEAST32_T@

/* The size of `uint_least64_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_LEAST64_T @H5_SIZEOF_UINT_LEAST64_T@

/* The size of `uint_least8_t', as computed by sizeof. */
//#define H5_SIZEOF_UINT_LEAST8_T @H5_SIZEOF_UINT_LEAST8_T@

/* The size of `unsigned', as computed by sizeof. */
//#define H5_SIZEOF_UNSIGNED @H5_SIZEOF_UNSIGNED@

/* The size of `_Quad', as computed by sizeof. */
//#define H5_SIZEOF__QUAD @H5_SIZEOF__QUAD@

/* The size of `__float128', as computed by sizeof. */
//#define H5_SIZEOF___FLOAT128 @H5_SIZEOF___FLOAT128@

/* The size of `__int64', as computed by sizeof. */
//#define H5_SIZEOF___INT64 @H5_SIZEOF___INT64@

/* Define to 1 if you have the ANSI C header files. */
//#define H5_STDC_HEADERS @H5_STDC_HEADERS@

/* Define if strict file format checks are enabled */
//#define H5_STRICT_FORMAT_CHECKS @H5_STRICT_FORMAT_CHECKS@

/* Define if your system supports pthread_attr_setscope(&attribute,
   PTHREAD_SCOPE_SYSTEM) call. */
//#define H5_SYSTEM_SCOPE_THREADS @H5_SYSTEM_SCOPE_THREADS@

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
//#define H5_TIME_WITH_SYS_TIME @H5_TIME_WITH_SYS_TIME@

/* Define using v1.6 public API symbols by default */
//#define H5_USE_16_API_DEFAULT @H5_USE_16_API_DEFAULT@

/* Define using v1.8 public API symbols by default */
//#define H5_USE_18_API_DEFAULT @H5_USE_18_API_DEFAULT@

/* Define if a memory checking tool will be used on the library, to cause
   library to be very picky about memory operations and also disable the
   internal free list manager code. */
//#define H5_USING_MEMCHECKER @H5_USING_MEMCHECKER@

/* Version number of package */
//#define H5_VERSION "@HDF5_PACKAGE_VERSION_STRING@"

/* Data accuracy is prefered to speed during data conversions */
//#define H5_WANT_DATA_ACCURACY @H5_WANT_DATA_ACCURACY@

/* Check exception handling functions during data conversions */
//#define H5_WANT_DCONV_EXCEPTION @H5_WANT_DCONV_EXCEPTION@

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if !defined(__APPLE__)
# ifndef WORDS_BIGENDIAN
#  undef WORDS_BIGENDIAN
# endif
#else
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#endif

//#define WORDS_BIGENDIAN @H5_WORDS_BIGENDIANR@

/* Number of bits in a file offset, on hosts where this is settable. */
#define H5__FILE_OFFSET_BITS

/* Define for large files, on AIX-style hosts. */
#define H5__LARGE_FILES

/* Define to empty if `const' does not conform to ANSI C. */
#define H5_const

/* Define to `long int' if <sys/types.h> does not define. */
#define H5_off_t

/* Define to `long' if <sys/types.h> does not define. */
#define H5_ptrdiff_t

/* Define to `unsigned long' if <sys/types.h> does not define. */
#define H5_size_t

/* Define to `long' if <sys/types.h> does not define. */
#define H5_ssize_t

#endif
