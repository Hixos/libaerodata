#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _MSC_VER
#ifdef EXTERNAL_FUNCTION_EXPORT
#define EXTLIB2_EXPORT __declspec(dllexport)
#else
#define EXTLIB2_EXPORT __declspec(dllimport)
#endif
#elif __GNUC__ >= 4
/* In gnuc, all symbols are by default exported. It is still often useful, to
 * not export all symbols but only the needed ones */
#define EXTLIB2_EXPORT __attribute__((visibility("default")))
#else
#define EXTLIB2_EXPORT
#endif

    /**
     * @brief Loads aerodata from a .npz file
     *
     * @param states_filename .npz file containing the state vectors
     * @param aerodata_filename .npz file containing aerodata values
     * @param state_keys List of state names
     * @param state_keys_size Lenght of @p state_keys
     * @param aerodata_keys List of aerodata fields to be loaded
     * @param aerodata_keys_size Length of @p aerodata_keys
     * @return Pointer to the aerodata object
     */
    EXTLIB2_EXPORT void* initAeroData(const char* states_filename,
                                      const char* aerodata_filename,
                                      const char** state_keys,
                                      size_t state_keys_size,
                                      const char** aerodata_keys,
                                      size_t aerodata_keys_size);

    /**
     * @brief Releases the provided aerodata object
     */
    EXTLIB2_EXPORT void closeAeroData(void* aerodata_ptr);

    /**
     * @brief Returns a list of aerodata values corresponding to the provided
     * state. The returned values are consistent with the aerodata_keys array
     * provided in the constructor
     *
     * @param aerodata_ptr Pointer to the aerodata object
     * @param state Aerodynamic state
     * @param state_size Size of the aerodynamic state array
     * @param aerodata Pointer to the first element of the array where data will be stored
     * @param aerodata_size size of the @p aerodata array
     */
    EXTLIB2_EXPORT void getAeroCoefficients(void* aerodata_ptr,
                                            const double* state,
                                            size_t state_size, double* aerodata,
                                            size_t aerodata_size);

#ifdef __cplusplus
}
#endif
