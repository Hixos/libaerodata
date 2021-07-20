#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <vector>

using dbl = std::numeric_limits<double>;

#include "Utils.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

using vd = vector<double>;

double f(vd data, int k)
{
    vd amp{-5, 2, 7, -3};
    vd freq{360 / 5, 360, 360 / 20, 360 / 10};
    vd phase{45, 50, 175, 247};

    double out = 0;
    for (size_t i = 0; i < data.size(); ++i)
    {
        out += amp[i] *
               sin(freq[i] * M_PI / 180 * data[i] + phase[i] * M_PI / 180);
    }

    return out;  // * (k + 1) + (k + 2);
}

vd linspace(double a, double b, size_t n)
{
    vd v;
    v.reserve(n);

    for (size_t i = 0; i < n; ++i)
    {
        v.push_back(a + (b - a) * (double)i / (n - 1.0));
    }

    return v;
}

vector<size_t> getIndex(size_t arr_index, const vector<vd>& vxq)
{
    size_t rank = vxq.size();
    vector<size_t> index(rank, 0);
    for (size_t i = 0; i < vxq.size() - 1; ++i)
    {
        index[rank - i - 1] = arr_index % vxq[rank - i - 1].size();
        arr_index =
            (arr_index - index[rank - i - 1]) / vxq[rank - i - 1].size();
    }
    index[0] = arr_index;
    return index;
}

int main()
{
    vector<vd> vx = {linspace(-5, 7, 10), linspace(0, 15, 15),
                     linspace(-10, -5, 5), linspace(-1, 6, 12)};

    vector<size_t> sizes;
    for (auto& x : vx)
    {
        sizes.push_back(x.size());
    }

    vector<Tensor> vt = {Tensor(sizes), Tensor(sizes), Tensor(sizes)};

    for (size_t it = 0; it < vt.size(); ++it)
    {
        auto& t      = vt[it];
        double* data = t.getData();
        size_t size  = t.getDataSize();
        for (size_t i = 0; i < size; ++i)
        {
            vector<size_t> index = t.getIndex(i);
            vd x;
            for (size_t j = 0; j < index.size(); ++j)
            {
                x.push_back(vx[j][index[j]]);
            }
            data[i] = f(x, it);
        }
    }

    vector<vd> vxq = {linspace(-7, 10, 9), linspace(-6, 20, 17),
                      linspace(-11, -2, 8), linspace(-4.5, 6.77, 12)};


    size_t dimq = 1;
    for (auto& xq : vxq)
    {
        dimq *= xq.size();
    }

    vector<ofstream> ofs;
    for (size_t i = 0; i < vt.size(); ++i)
    {
        ofs.emplace_back("interp" + to_string(i) + ".csv");
        ofs[ofs.size() - 1].precision(dbl::max_digits10 + 2);
        ofs[ofs.size() - 1] << std::scientific;
        for (size_t j = 0; j < vx.size(); ++j)
        {
            ofs[ofs.size() - 1] << "x" << j + 1 << ",";
        }
        ofs[ofs.size() - 1] << "interp\n";
    }

    for (size_t i = 0; i < dimq; ++i)
    {
        vector<size_t> index = getIndex(i, vxq);
        vd xq;
        for (size_t j = 0; j < vxq.size(); ++j)
        {
            xq.push_back(vxq[j][index[j]]);
        }

        vd data = multiInterpN(vt, vx, xq);

        for (size_t j = 0; j < data.size(); ++j)
        {
            for (auto x : xq)
            {
                ofs[j] << x << ",";
            }
            ofs[j] << data[j] << "\n";
        }
    }
}