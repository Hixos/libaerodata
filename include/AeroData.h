#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Tensor.h"

using std::vector;

class AeroData
{
public:
    enum class Interpolation : uint8_t
    {
        NEAREST,
        LINEAR
    };

    AeroData(const vector<vector<double>>& state_arrays,
             const vector<Tensor>& aerodata_tensors);

    vector<double> getAeroData(
        const vector<double>& state,
        Interpolation interp = Interpolation::NEAREST) const;

    vector<double> getAeroData(const vector<size_t>& index) const;
    vector<double> getAeroData(size_t flat_index) const;

    void getAeroData(const double* state, size_t state_size, double* coeffs,
                     size_t coeff_size,
                     Interpolation interp = Interpolation::NEAREST) const;

    const vector<vector<double>>& getStates() { return state_arrays; };

    vector<double> getState(const vector<size_t>& index) const;
    vector<double> getState(size_t flat_index) const;

    size_t getTensorFlatSize() { return aerodata_tensors[0].getDataSize(); }

private:
    vector<Tensor> aerodata_tensors;
    vector<vector<double>> state_arrays;
};