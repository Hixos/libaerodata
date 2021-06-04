#include "AeroData.h"

#include <cnpy.h>

#include <cassert>
#include <cstdio>

#include "Utils.h"

AeroData::AeroData(const vector<vector<double>>& state_arrays,
                   const vector<Tensor>& coefficients)
    : coeff_data(coefficients), state_arrays(state_arrays)
{
}

vector<double> AeroData::getCoefficients(const vector<double>& state,
                                         Interpolation interp) const
{
    assert(state.size() == state_arrays.size());

    // TODO: Add other interpolation methods
    (void)interp;
    vector<size_t> indices;
    indices.reserve(state_arrays.size());

    for (size_t i = 0; i < state.size(); i++)
    {
        indices.push_back(nearestBinarySearch(state_arrays[i], state[i]));
    }

    // All coefficient tensors are the same size
    size_t arr_index = coeff_data[0].getArrayIndex(indices);

    vector<double> coeffs;
    coeffs.reserve(coeff_data.size());

    for (const Tensor& t : coeff_data)
    {
        coeffs.push_back(t(arr_index));
    }

    return coeffs;
}

void AeroData::getCoefficients(const double* state, size_t state_size,
                               double* coeffs, size_t coeff_size,
                               Interpolation interp) const
{
    assert(state_size == state_arrays.size());
    assert(coeff_size == coeff_data.size());

    if (interp == Interpolation::NEAREST)
    {
        vector<size_t> indices;
        indices.reserve(state_arrays.size());

        for (size_t i = 0; i < state_size; ++i)
        {
            indices.push_back(nearestBinarySearch(state_arrays[i], state[i]));
        }

        // All coefficient tensors are the same size
        size_t arr_index = coeff_data[0].getArrayIndex(indices);

        for (size_t i = 0; i < coeff_size; ++i)
        {
            coeffs[i] = coeff_data[i](arr_index);
        }
    }
    else
    {
        vector<double> v_state(state, state + state_size);
        vector<double> v_coeffs =
            multiInterpN(coeff_data, state_arrays, v_state);
        memcpy(coeffs, v_coeffs.data(), coeff_size * sizeof(double));
    }
}