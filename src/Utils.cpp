#include "Utils.h"

#include <cmath>

double interpN(const Tensor& v, const vector<vector<double>>& x,
               const vector<double>& xq)
{
    static_assert(std::numeric_limits<double>::has_quiet_NaN == true,
                  "Double does not implemetn quit nans");

    size_t rank = v.getRank();
    if (rank != xq.size() || rank != x.size())
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    vector<size_t> base_indices;
    base_indices.reserve(rank);

    for (size_t i = 0; i < rank; ++i)
    {
        base_indices.push_back(previousBinarySearch(x[i], xq[i]));
    }

    size_t vals_size = 1 << (rank + 1);  // 2 ^ (rank + 1)
    vector<double> vals(vals_size, 0);

    vector<size_t> indices(rank, 0);
    for (size_t i = 0; i < vals_size; ++i)
    {
        for (size_t j = 0; j < rank; ++j)
        {
            indices[j] = base_indices[j] + ((i >> j) & 0x01);
        }

        vals[i] = v(indices);
    }

    for (size_t i = 0; i < rank; ++i)
    {
        // Outside of the available values: closest value extrapolation
        if (xq[i] >= x[i][x[i].size() - 1] || xq[i] <= x[i][0])
        {
            continue;
        }
        size_t half_step = (i + 1);

        double dx = xq[i] - x[i][indices[i]];
        double h  = x[i][indices[i] + 1] - x[i][indices[i]];
        for (size_t j = 0; j < vals_size; j += half_step * 2)
        {
            vals[j] = interp1(dx, h, vals[j], vals[j + half_step]);
        }
    }

    return vals[0];
}