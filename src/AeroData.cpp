#include "AeroData.h"

#include <cnpy.h>

#include <cassert>
#include <cstdio>
#include <cstring>

AeroData::AeroData(const vector<vector<double>>& state_arrays,
                   const vector<Tensor>& aerodata_tensors)
    : aerodata_tensors(aerodata_tensors), state_arrays(state_arrays),
      interpolator(aerodata_tensors.size(), state_arrays.size())
{
}

vector<double> AeroData::getAeroData(const vector<double>& state,
                                     Interpolation interp)
{
    assert(state.size() == state_arrays.size());

    vector<double> coeffs(aerodata_tensors.size(), 0);
    getAeroData(state.data(), state.size(), coeffs.data(), coeffs.size(),
                interp);

    return coeffs;
}

vector<double> AeroData::getAeroData(const vector<size_t>& index)
{
    vector<double> aerodata;
    aerodata.reserve(aerodata_tensors.size());

    for (size_t i = 0; i < aerodata_tensors.size(); ++i)
    {
        aerodata.push_back(aerodata_tensors[i](index));
    }

    return aerodata;
}

vector<double> AeroData::getAeroData(size_t flat_index)
{
    vector<size_t> index = aerodata_tensors[0].getIndex(flat_index);
    return getAeroData(index);
}

void AeroData::getAeroData(const double* state, size_t state_size,
                           double* aerodata, size_t aerodata_size,
                           Interpolation interp)
{
    assert(state_size == state_arrays.size());
    assert(aerodata_size == aerodata_tensors.size());

    if (interp == Interpolation::NEAREST)
    {
        vector<size_t> indices;
        indices.reserve(state_arrays.size());

        for (size_t i = 0; i < state_size; ++i)
        {
            indices.push_back(nearestBinarySearch(state_arrays[i], state[i]));
        }

        // All coefficient tensors are the same size
        size_t arr_index = aerodata_tensors[0].getArrayIndex(indices);

        for (size_t i = 0; i < aerodata_size; ++i)
        {
            aerodata[i] = aerodata_tensors[i](arr_index);
        }
    }
    else
    {
        vector<double> v_state(state, state + state_size);
        // vector<double> v_coeffs =
        //     multiInterpN(aerodata_tensors, state_arrays, v_state);
        vector<double> v_coeffs =
            interpolator.interpolate(aerodata_tensors, state_arrays, v_state);

        memcpy(aerodata, v_coeffs.data(), aerodata_size * sizeof(double));
    }
}

vector<double> AeroData::getState(const vector<size_t>& index) const
{
    vector<double> state;
    state.reserve(state_arrays.size());

    for (size_t i = 0; i < state_arrays.size(); ++i)
    {
        state.push_back(state_arrays[i][index[i]]);
    }

    return state;
}

vector<double> AeroData::getState(size_t flat_index) const
{
    vector<size_t> index = aerodata_tensors[0].getIndex(flat_index);
    return getState(index);
}

const vector<vector<double>>& AeroData::getStateArrays()
{
    return state_arrays;
}