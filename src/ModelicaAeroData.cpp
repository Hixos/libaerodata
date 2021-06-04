#include "ModelicaAeroData.h"

#include <cnpy.h>

#include <cstdio>
#include <string>
#include <vector>

#include "AeroData.h"

using namespace std;

void* initAeroData(const char* states_filename, const char* coeffs_filename,
                   const char** state_list, size_t state_list_size,
                   const char** coeff_list, size_t coeff_list_size)
{
    vector<vector<double>> state_arrs;
    vector<Tensor> coefficients;

    for (int i = 0; i < state_list_size; ++i)
    {
        cnpy::NpyArray state =
            cnpy::npz_load(string{states_filename}, string{state_list[i]});
        state_arrs.push_back(state.as_vec<double>());
    }

    for (int i = 0; i < coeff_list_size; ++i)
    {
        cnpy::NpyArray coeff =
            cnpy::npz_load(string{coeffs_filename}, string{coeff_list[i]});
        coefficients.emplace_back(coeff.shape, coeff.data<double>());
    }

    AeroData* aerodata = new AeroData(state_arrs, coefficients);

    return static_cast<void*>(aerodata);
}

void closeAeroData(void* aerodata_ptr)
{
    AeroData* aerodata = static_cast<AeroData*>(aerodata_ptr);

    delete aerodata;
}

void getAeroCoefficients(void* aerodata_ptr, const double* state,
                         size_t state_size, double* coefficients,
                         size_t coeff_size)
{
    AeroData* aerodata = static_cast<AeroData*>(aerodata_ptr);

    aerodata->getCoefficients(state, state_size, coefficients, coeff_size,
                              AeroData::Interpolation::LINEAR);
}