#ifndef TB_CONFIG_H
#define TB_CONFIG_H

// packages
// [packages]

// major version
#define TB_CONFIG_VERSION_MAJOR         [major]

// minor version
#define TB_CONFIG_VERSION_MINOR         [minor]

// alter version
#define TB_CONFIG_VERSION_ALTER         [alter]

// build version
#define TB_CONFIG_VERSION_BUILD         [build]

// small
#define TB_CONFIG_SMALL                 [small]

// os
#define TB_CONFIG_OS_WINDOWS
//#define TB_CONFIG_OS_LIKE_UNIX

// api
//#define TB_CONFIG_API_HAVE_POSIX
//#define TB_CONFIG_API_HAVE_SYSTEMV

// type
#define TB_CONFIG_TYPE_FLOAT

// memory
//#define TB_CONFIG_MEMORY_UNALIGNED_ACCESS_ENABLE

// trace
//#define TB_CONFIG_TRACE_INFO_ONLY

// exception
#define TB_CONFIG_EXCEPTION_ENABLE

// libc
#define TB_CONFIG_LIBC_HAVE_MEMCPY
#define TB_CONFIG_LIBC_HAVE_MEMSET
#define TB_CONFIG_LIBC_HAVE_MEMMOV
#define TB_CONFIG_LIBC_HAVE_MEMCMP
#define TB_CONFIG_LIBC_HAVE_STRCAT
#define TB_CONFIG_LIBC_HAVE_STRNCAT
#define TB_CONFIG_LIBC_HAVE_STRCPY
#define TB_CONFIG_LIBC_HAVE_STRNCPY
//#define TB_CONFIG_LIBC_HAVE_STRLCPY
#define TB_CONFIG_LIBC_HAVE_STRLEN 
#define TB_CONFIG_LIBC_HAVE_STRNLEN
#define TB_CONFIG_LIBC_HAVE_STRSTR
//#define TB_CONFIG_LIBC_HAVE_STRISTR
#define TB_CONFIG_LIBC_HAVE_STRCMP
#define TB_CONFIG_LIBC_HAVE_STRICMP
#define TB_CONFIG_LIBC_HAVE_STRNCMP
#define TB_CONFIG_LIBC_HAVE_STRNICMP
#define TB_CONFIG_LIBC_HAVE_WCSCAT
#define TB_CONFIG_LIBC_HAVE_WCSNCAT
#define TB_CONFIG_LIBC_HAVE_WCSCPY
#define TB_CONFIG_LIBC_HAVE_WCSNCPY
//#define TB_CONFIG_LIBC_HAVE_WCSLCPY
#define TB_CONFIG_LIBC_HAVE_WCSLEN 
#define TB_CONFIG_LIBC_HAVE_WCSNLEN
#define TB_CONFIG_LIBC_HAVE_WCSSTR
//#define TB_CONFIG_LIBC_HAVE_WCSISTR
#define TB_CONFIG_LIBC_HAVE_WCSCMP
#define TB_CONFIG_LIBC_HAVE_WCSICMP
#define TB_CONFIG_LIBC_HAVE_WCSNCMP
#define TB_CONFIG_LIBC_HAVE_WCSNICMP
#define TB_CONFIG_LIBC_HAVE_WCSTOMBS
#define TB_CONFIG_LIBC_HAVE_MBSTOWCS
#define TB_CONFIG_LIBC_HAVE_GMTIME
#define TB_CONFIG_LIBC_HAVE_MKTIME
#define TB_CONFIG_LIBC_HAVE_LOCALTIME
//#define TB_CONFIG_LIBC_HAVE_SETJMP

// libm
#define TB_CONFIG_LIBM_HAVE_SINCOS
//#define TB_CONFIG_LIBM_HAVE_SINCOSF
//#define TB_CONFIG_LIBM_HAVE_LOG2
//#define TB_CONFIG_LIBM_HAVE_LOG2F

// module
#define TB_CONFIG_MODULE_HAVE_XML
#define TB_CONFIG_MODULE_HAVE_ZIP
#define TB_CONFIG_MODULE_HAVE_ASIO
#define TB_CONFIG_MODULE_HAVE_OBJECT
#define TB_CONFIG_MODULE_HAVE_CHARSET
#define TB_CONFIG_MODULE_HAVE_DATABASE

#endif
