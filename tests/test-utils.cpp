#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <vector>

#include "Utils.h"

using namespace std;
using namespace Catch::literals;

TEST_CASE("Nearest Binary Search")
{
    SECTION("1 element state_arr")
    {
        vector<double> state_arr = {0};

        REQUIRE(nearestBinarySearch(state_arr, -1.0) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 100.0) == state_arr.size() - 1);

        REQUIRE(nearestBinarySearch(state_arr, 0.0) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 0.1) == 0);
    }

    SECTION("2 element state_arr")
    {
        vector<double> state_arr = {0, 1};

        REQUIRE(nearestBinarySearch(state_arr, -1.0) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 100.0) == state_arr.size() - 1);

        REQUIRE(nearestBinarySearch(state_arr, 0.0) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 0.1) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 0.9) == 1);
        REQUIRE(nearestBinarySearch(state_arr, 1.1) == 1);
    }

    SECTION("Long state_arr")
    {
        vector<double> state_arr = {0, 0.5, 1, 2, 5, 7.5, 10, 15, 20, 30, 50};
        REQUIRE(nearestBinarySearch(state_arr, -1.0) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 100.0) == state_arr.size() - 1);

        REQUIRE(nearestBinarySearch(state_arr, 0.0) == 0);
        REQUIRE(nearestBinarySearch(state_arr, 0.1) == 0);

        REQUIRE(nearestBinarySearch(state_arr, 1.0) == 2);

        REQUIRE(nearestBinarySearch(state_arr, 12.5) == 6);

        REQUIRE(nearestBinarySearch(state_arr, 15.0) == 7);
        REQUIRE(nearestBinarySearch(state_arr, 50.0) == state_arr.size() - 1);
    }
}

TEST_CASE("Previous Binary Search")
{
    SECTION("1 element state_arr")
    {
        vector<double> state_arr = {0};

        REQUIRE(previousBinarySearch(state_arr, -1.0) == 0);
        REQUIRE(previousBinarySearch(state_arr, 100.0) == state_arr.size() - 1);

        REQUIRE(previousBinarySearch(state_arr, 0.0) == 0);
        REQUIRE(previousBinarySearch(state_arr, 0.1) == 0);
    }

    SECTION("2 element state_arr")
    {
        vector<double> state_arr = {0, 1};

        REQUIRE(previousBinarySearch(state_arr, -1.0) == 0);
        REQUIRE(previousBinarySearch(state_arr, 100.0) == state_arr.size() - 1);

        REQUIRE(previousBinarySearch(state_arr, 0.0) == 0);
        REQUIRE(previousBinarySearch(state_arr, 0.1) == 0);
        REQUIRE(previousBinarySearch(state_arr, 0.9) == 0);
        REQUIRE(previousBinarySearch(state_arr, 1.1) == 1);
    }

    SECTION("Long state_arr")
    {
        vector<double> state_arr = {0, 0.5, 1, 2, 5, 7.5, 10, 15, 20, 30, 50};
        REQUIRE(previousBinarySearch(state_arr, -1.0) == 0);
        REQUIRE(previousBinarySearch(state_arr, 100.0) == state_arr.size() - 1);

        REQUIRE(previousBinarySearch(state_arr, 0.0) == 0);
        REQUIRE(previousBinarySearch(state_arr, 0.1) == 0);

        REQUIRE(previousBinarySearch(state_arr, 1.0) == 2);

        REQUIRE(previousBinarySearch(state_arr, 12.5) == 6);
        REQUIRE(previousBinarySearch(state_arr, 14.999) == 6);

        REQUIRE(previousBinarySearch(state_arr, 15.0) == 7);
        REQUIRE(previousBinarySearch(state_arr, 50.0) == state_arr.size() - 1);
    }
}

TEST_CASE("interp1")
{
    float v1 = 10;
    float x1 = 5;

    float v2 = 20;
    float x2 = 15;

    REQUIRE(interp1<float>(x1, x1, x2, v1, v2) == 10);
    REQUIRE(interp1<float>(x2, x1, x2, v1, v2) == 20);
    REQUIRE(interp1<float>(6.0f, x1, x2, v1, v2) == 11_a);
    REQUIRE(interp1<float>(10.0f, x1, x2, v1, v2) == 15_a);
}

TEST_CASE("interpN")
{
    SECTION("1 D")
    {
        vector<double> x = {0, 1, 3, 7, 11};
        vector<double> v = {5, 10, 20, 50, 100};

        vector<size_t> dims = {x.size()};
        Tensor t{dims, v.data()};
        REQUIRE(interpN<double>(t, {x}, {0}) == 5);
        REQUIRE(interpN<double>(t, {x}, {1}) == 10);
        REQUIRE(interpN<double>(t, {x}, {11}) == 100);

        REQUIRE(interpN<double>(t, {x}, {0.5}) == 7.5_a);
        REQUIRE(interpN<double>(t, {x}, {9}) == 75_a);

        REQUIRE(interpN<double>(t, {x}, {20}) == 100);
        REQUIRE(interpN<double>(t, {x}, {-5}) == 5);
    }

    SECTION("2 D")
    {
        vector<double> x1 = {0, 1, 3, 7, 11};
        vector<double> x2 = {-5, -2, 1, 4};

        vector<double> v;
        v.reserve(x1.size() * x2.size());

        for (size_t i = 0; i < x1.size(); ++i)
        {
            for (size_t j = 0; j < x2.size(); ++j)
            {
                v.push_back(x1[i] + x2[j] * 2 + 1);
            }
        }

        Tensor t{{x1.size(), x2.size()}, v.data()};

        REQUIRE(interpN<double>(t, {x1, x2}, {0, -5}) == -9);
        REQUIRE(interpN<double>(t, {x1, x2}, {11, 4}) == 20);

        REQUIRE(interpN<double>(t, {x1, x2}, {0.5, 5}) == 9.5_a);
        REQUIRE(interpN<double>(t, {x1, x2}, {-1, -3.5}) == -6_a);
        REQUIRE(interpN<double>(t, {x1, x2}, {12, 5}) == 20);
    }

    SECTION("multi Interp")
    {
        vector<double> x1 = {0, 1, 3, 7, 11};
        vector<double> x2 = {-5, -2, 1, 4};

        vector<double> v1;
        vector<double> v2;
        v1.reserve(x1.size() * x2.size());
        v2.reserve(x1.size() * x2.size());

        for (size_t i = 0; i < x1.size(); ++i)
        {
            for (size_t j = 0; j < x2.size(); ++j)
            {
                v1.push_back(x1[i] + x2[j] * 2 + 1);
                v2.push_back(x1[i] + x2[j] * 2 + 2);
            }
        }

        vector<Tensor> data;
        data.emplace_back(vector<size_t>{x1.size(), x2.size()}, v1.data());
        data.emplace_back(vector<size_t>{x1.size(), x2.size()}, v2.data());

        REQUIRE(multiInterpN<double>(data, {x1, x2}, {0, -5}) == vector<double>{-9, -8});
        REQUIRE(multiInterpN<double>(data, {x1, x2}, {11, 4}) == vector<double>{20, 21});

        REQUIRE(multiInterpN<double>(data, {x1, x2}, {0.5, 5}) == vector<double>{9.5, 10.5});
        REQUIRE(multiInterpN<double>(data, {x1, x2}, {-1, -3.5}) == vector<double>{-6, -5});
        REQUIRE(multiInterpN<double>(data, {x1, x2}, {12, 5}) == vector<double>{20, 21});
    }
}