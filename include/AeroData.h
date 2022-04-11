#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Utils.h"
#include "Tensor.h"

using std::vector;

/**
 * @brief Class providing methods for accessing aerodynamic data tensors
 * 
 */
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
        Interpolation interp = Interpolation::NEAREST);

    vector<double> getAeroData(const vector<size_t>& index);
    vector<double> getAeroData(size_t flat_index);

    void getAeroData(const double* state, size_t state_size, double* coeffs,
                     size_t coeff_size,
                     Interpolation interp = Interpolation::NEAREST);

    const vector<vector<double>>& getStates() { return state_arrays; };

    vector<double> getState(const vector<size_t>& index) const;
    vector<double> getState(size_t flat_index) const;

    const vector<vector<double>>& getStateArrays();

    size_t getTensorFlatSize() { return aerodata_tensors[0].getDataSize(); }

private:
    vector<Tensor> aerodata_tensors;
    vector<vector<double>> state_arrays;
    MultiInterpolator<double> interpolator;
};