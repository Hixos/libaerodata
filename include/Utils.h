#pragma once

#include <cassert>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include "Tensor.h"

using std::vector;

template <typename T>
inline size_t nearestBinarySearch(const vector<T>& state_arr, T state)
{
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

    size_t size = state_arr.size();
    if (state <= state_arr[0])
        return 0;
    if (state >= state_arr[size - 1])
        return size - 1;

    size_t l = 0;
    size_t r = size - 1;
    size_t c = l + (r - l) / 2;

    while (l != r - 1)
    {
        if (state_arr[c] <= state)
        {
            if (state_arr[c + 1] >= state)
                break;
            l = c;
        }
        else
            r = c;

        c = l + (r - l) / 2;
    }

    if (abs(state_arr[c + 1] - state) < abs(state_arr[c] - state))
        return c + 1;
    else
        return c;
}

template <typename T>
inline size_t previousBinarySearch(const vector<T>& state_arr, T state)
{
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

    size_t size = state_arr.size();
    if (state <= state_arr[0])
        return 0;
    if (state >= state_arr[size - 1])
        return size - 1;

    size_t l = 0;
    size_t r = size - 1;
    size_t c = l + (r - l) / 2;

    while (l != r - 1)
    {
        if (state_arr[c] <= state)
        {
            if (state_arr[c + 1] >= state)
                break;
            l = c;
        }
        else
            r = c;

        c = l + (r - l) / 2;
    }

    return c;
}

template <typename T>
inline T interp1(T dx, T h, T v1, T v2)
{
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");

    return v1 + (v2 - v1) * dx / h;
}

template <typename T>
inline T interp1(T x, T x1, T x2, T v1, T v2)
{
    return interp1(x - x1, x2 - x1, v1, v2);
}

template <typename T>
inline T interpN(const Tensor& v, const vector<vector<T>>& x,
                 const vector<T>& xq)
{
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    static_assert(std::numeric_limits<T>::has_quiet_NaN == true,
                  "T does not implement quiet nans");

    size_t rank = v.getRank();
    if (rank != xq.size() || rank != x.size())
    {
        return std::numeric_limits<T>::quiet_NaN();
    }

    vector<size_t> base_indices;
    base_indices.reserve(rank);

    for (size_t i = 0; i < rank; ++i)
    {
        base_indices.push_back(previousBinarySearch(x[i], xq[i]));
    }

    size_t vals_size = 1 << rank;  // 2 ^ rank
    vector<T> vals(vals_size, 0);

    vector<size_t> indices(rank, 0);
    for (size_t i = 0; i < vals_size; ++i)
    {
        for (size_t j = 0; j < rank; ++j)
        {
            indices[j] =
                std::min(x[j].size() - 1, base_indices[j] + ((i >> j) & 0x01));
        }

        vals[i] = v(indices);
    }

    for (size_t i = 0; i < rank; ++i)
    {
        // Outside of the available range: closest value extrapolation
        if (xq[i] >= x[i][x[i].size() - 1] || xq[i] <= x[i][0])
        {
            continue;
        }
        size_t half_step = i << 1;

        T dx = xq[i] - x[i][base_indices[i]];
        T h  = x[i][base_indices[i] + 1] - x[i][base_indices[i]];
        for (size_t j = 0; j < vals_size; j += half_step * 2)
        {
            vals[j] = interp1(dx, h, vals[j], vals[j + half_step]);
        }
    }

    return vals[0];
}

template <typename T>
inline vector<T> multiInterpN(const vector<Tensor>& v,
                              const vector<vector<T>>& x, const vector<T>& xq)
{
    static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    static_assert(std::numeric_limits<T>::has_quiet_NaN == true,
                  "T does not implement quiet nans");

    size_t rank = v.at(0).getRank();
    if (rank != xq.size() || rank != x.size())
    {
        return vector<T>(v.size(), std::numeric_limits<T>::quiet_NaN());
    }

    vector<size_t> base_indices;
    base_indices.reserve(rank);

    for (size_t i = 0; i < rank; ++i)
    {
        base_indices.push_back(previousBinarySearch(x[i], xq[i]));
    }

    size_t vals_size = 1 << rank;  // 2 ^ rank
    // vector<T> vals(vals_size, 0);
    vector<vector<T>> vals(v.size(), vector<T>(vals_size, 0));

    vector<size_t> indices(rank, 0);
    for (size_t i = 0; i < vals_size; ++i)
    {
        for (size_t j = 0; j < rank; ++j)
        {
            indices[j] =
                std::min(x[j].size() - 1, base_indices[j] + ((i >> j) & 0x01));
        }

        for (size_t j = 0; j < v.size(); ++j)
        {
            vals[j][i] = v[j](indices);
        }
    }

    for (size_t i = 0; i < rank; ++i)
    {
        // Outside of the available range: closest value extrapolation
        if (xq[i] >= x[i][x[i].size() - 1] || xq[i] <= x[i][0])
        {
            continue;
        }
        
        assert(base_indices[i] + 1< x[i].size());

        size_t half_step = 1 << i;

        T dx = xq[i] - x[i][base_indices[i]];
        T h  = x[i][base_indices[i] + 1] - x[i][base_indices[i]];

        assert(dx >= 0);
        assert(h > 0);

        for (size_t v_i = 0; v_i < vals.size(); ++v_i)
        {
            for (size_t j = 0; j < vals_size; j += half_step * 2)
            {
                vals[v_i][j] =
                    interp1(dx, h, vals[v_i][j], vals[v_i][j + half_step]);
            }
        }
    }

    vector<T> val_out;
    val_out.reserve(v.size());
    for (size_t i = 0; i < vals.size(); ++i)
    {
        val_out.push_back(vals[i][0]);
    }
    return val_out;
}