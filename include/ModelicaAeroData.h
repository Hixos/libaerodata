#pragma once

#include <stddef.h>

#ifdef __cplusplus 
extern "C" { 
#endif
#ifdef _MSC_VER
 #ifdef EXTERNAL_FUNCTION_EXPORT 
    #define EXTLIB2_EXPORT __declspec( dllexport ) 
 #else 
    #define EXTLIB2_EXPORT __declspec( dllimport )
 #endif 
#elif __GNUC__ >= 4
/* In gnuc, all symbols are by default exported. It is still often useful, to not export all symbols but only the needed ones */
 #define EXTLIB2_EXPORT __attribute__ ((visibility("default")))
#else 
 #define EXTLIB2_EXPORT 
#endif

EXTLIB2_EXPORT void* initAeroData(const char* states_filename, const char* coeffs_filename,
                       const char** state_list, size_t state_list_size,
                       const char** coeff_list, size_t coeff_list_size);

EXTLIB2_EXPORT void closeAeroData(void* aerodata_ptr);

EXTLIB2_EXPORT void getAeroCoefficients(void* aerodata_ptr, const double* state,
                             size_t state_size, double* coefficients,
                             size_t coeff_size);

#ifdef __cplusplus
}
#endif
