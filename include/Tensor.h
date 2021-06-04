#pragma once

#include <cstddef>
#include <vector>

using std::vector;

class Tensor
{
public:
    /**
     * @brief Creates a new Tensor with the provided dimensions
     */
    Tensor(vector<size_t> dimensions);

    /**
     * @brief Creates a new Tensor with the provided dimensions and fills it
     * with a copy of the provided data
     *
     * @param dimensions vector of dimensions of the tensor
     * @param data Row major data array
     */
    Tensor(vector<size_t> dimensions, double* data);

    Tensor(const Tensor& other);
    Tensor(Tensor&& other);

    ~Tensor();

    Tensor& operator=(const Tensor& other);
    Tensor& operator=(Tensor&& other);

    double operator()(const vector<size_t>& index) const;
    double& operator()(const vector<size_t>& index);

    double operator()(size_t index) const;
    double& operator()(size_t index);

    vector<size_t> getIndex(size_t arr_index) const;
    size_t getArrayIndex(vector<size_t> index) const;

    size_t getRank() const { return dimensions.size(); }
    vector<size_t> getDimensions() const { return dimensions; }
    double* getData() { return data; };
    size_t getDataSize() { return data_size; };

private:
    double* data = nullptr;
    vector<size_t> dimensions;
    size_t data_size;
};