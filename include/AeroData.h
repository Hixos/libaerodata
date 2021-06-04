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
             const vector<Tensor>& coefficients);

    vector<double> getCoefficients(
        const vector<double>& state,
        Interpolation interp = Interpolation::NEAREST) const;

    void getCoefficients(const double* state, size_t state_size, double* coeffs,
                         size_t coeff_size,
                         Interpolation interp = Interpolation::NEAREST) const;

private:
    vector<Tensor> coeff_data;
    vector<vector<double>> state_arrays;
};