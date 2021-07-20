#include "Tensor.h"

#include <cassert>
#include <cstring>
#include <stdexcept>

Tensor::Tensor(vector<size_t> dimensions) : dimensions(dimensions)
{
    if (dimensions.size() == 0)
        throw std::invalid_argument(
            "The table must be at least one-dimensional");

    data_size = 1;
    for (size_t dim : dimensions)
        data_size *= dim;

    data = new double[data_size];
}

Tensor::Tensor(vector<size_t> dimensions, double* data) : dimensions(dimensions)
{
    if (dimensions.size() == 0)
        throw std::invalid_argument(
            "The table must be at least one-dimensional");

    data_size = 1;
    for (size_t dim : dimensions)
        data_size *= dim;

    this->data = new double[data_size];
    memcpy(this->data, data, data_size * sizeof(double));
}

Tensor::Tensor(const Tensor& other)
    : dimensions(other.dimensions), data_size(other.data_size)
{
    data = new double[data_size];
    memcpy(data, other.data, data_size * sizeof(double));
}

Tensor::Tensor(Tensor&& other)
    : data(other.data), dimensions(other.dimensions),
      data_size(other.data_size)
{
    other.data = nullptr;
}

Tensor& Tensor::operator=(const Tensor& other)
{
    dimensions   = other.dimensions;
    data_size = other.data_size;
    delete[] data;
    data = new double[data_size];
    memcpy(data, other.data, data_size * sizeof(double));

    return *this;
}

Tensor& Tensor::operator=(Tensor&& other)
{
    dimensions   = other.dimensions;
    data_size = other.data_size;
    delete[] data;
    data       = other.data;
    other.data = nullptr;

    return *this;
}

Tensor::~Tensor()
{
    if (data != nullptr)
        delete[] data;
}

double Tensor::operator()(const vector<size_t>& index) const
{
    assert(index.size() == dimensions.size());
    return (*this)(getArrayIndex(index));
}

double Tensor::operator()(size_t arr_index) const
{
    assert(arr_index <= data_size);
    return data[arr_index];
}

double& Tensor::operator()(const vector<size_t>& index)
{
    assert(index.size() == dimensions.size());
    return (*this)(getArrayIndex(index));
}

double& Tensor::operator()(size_t arr_index)
{
    assert(arr_index <= data_size);
    return data[arr_index];
}

vector<size_t> Tensor::getIndex(size_t arr_index) const
{
    size_t rank = getRank();
    vector<size_t> index(rank, 0);
    for (size_t i = 0; i < dimensions.size() - 1; ++i)
    {
        index[rank - i - 1] = arr_index % dimensions[rank - i - 1];
        arr_index =
            (arr_index - index[rank - i - 1]) / dimensions[rank - i - 1];
    }
    index[0] = arr_index;
    return index;
}

size_t Tensor::getArrayIndex(vector<size_t> index) const
{
    size_t i;
    size_t arr_index = 0;

    for (i = 0; i < dimensions.size() - 1; ++i)
    {
        arr_index = (arr_index + index[i]) * dimensions[i + 1];
    }
    arr_index += index[i];

    return arr_index;
}
