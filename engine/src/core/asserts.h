#pragma once

#include "defines.h"

// Poder desabilitar assertions
#define KASSERTIONS_ENABLED 1

#ifdef KASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBrack() __debugbreak()
#else
#define debugBrack() __builtin_trap()
#endif

KAPI void report_assertion_failure(const char* expresion, const char* message, const char* file, i32 line);

#define KASSERT(expr)                                                \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBrack();                                            \
        }                                                            \
    } 


#define KASSERT_MSG(expr, message)                                        \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBrack();                                                 \
        }                                                                 \
    } 

#ifdef _DEBUG
#define KASSERT_DEBUG(expr)                                          \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBrack();                                            \
        }                                                            \
    }
#else
#define KASSERT_DEBUG(expr) 
#endif

#else

#define KASSERT(expr)
#define KASSERT_MSG(expr, message)
#define KASSERT_DEBUG(expr)

#endif