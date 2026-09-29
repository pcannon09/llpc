/**
 * predefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 */

#ifndef INCLUDE_LLPCL_PREDEFINES_H_
#define INCLUDE_LLPCL_PREDEFINES_H_

#ifdef __cplusplus
#	define LLPC_PREDEFINES_OPEN			extern "C" {
#	define LLPC_PREDEFINES_CLOSE		}
#else
#	define LLPC_PREDEFINES_OPEN
#	define LLPC_PREDEFINES_CLOSE
#endif

LLPC_PREDEFINES_OPEN

// Project setup
#define LLPC_DEFAULT_C_STD			201112L

// Versioning
#define LLPC_VERSION_MAJOR            0
#define LLPC_VERSION_MINOR            0
#define LLPC_VERSION_PATCH            1

#define LLPC_VERSION_STD              0

// Version states:
// * dev
// * beta
// * build
#define LLPC_VERSION_STATE          "dev"

#define LLPC_VERSION                ((LLPC_VERSION_MAJOR<<16)|(LLPC_VERSION_MINOR<<8)|(LLPC_VERSION_PATCH)|(LLPC_VERSION_STATE << 24))

#define LLPC_VERSION_CHECK(LLPC_VERSION_MAJOR, LLPC_VERSION_MINOR, LLPC_VERSION_PATCH, LLPC_VERSION_STATE) \
    (((LLPC_VERSION_MAJOR)<<16)|((LLPC_VERSION_MINOR)<<8)|(LLPC_VERSION_PATCH)|((LLPC_VERSION_STATE) << 24))

// Macro utils
#define LLPC_STRINGIFY(x) #x
#define LLPC_TOSTRING(x) LLPC_STRINGIFY(x)

#define LLPC_UNUSED(x)		(void)x

#ifndef LLPC_DEV
#   define LLPC_DEV      1
#endif

#if __STDC_VERSION__ >= 202311L
# 	define __LLPC_HAS_C23	1
#else
# 	define __LLPC_HAS_C23	0
#endif

#define LLPCAPI			__attribute__((visibility("default")))
#define LLPC_ALIGN(_x)	__attribute__((aligned(_x)))

LLPC_PREDEFINES_CLOSE

#endif  // INCLUDE_LLPCL_PREDEFINES_H_

