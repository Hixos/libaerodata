#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "AeroData.h"
#include "ModelicaAeroData.h"

using namespace std;

const char* STATES_STR[] = {"alpha",      "mach",       "beta",
                            "altitude",   "fin2delta1", "fin2delta2",
                            "fin2delta3", "fin2delta4"};
const char* COEFF_STR[]  = {
    "CA",        "CAQ",       "CD",        "CL",        "CLCD",
    "CLL",       "CLLB",      "CLLP",      "CLLR",      "CLN",
    "CLNB",      "CLNP",      "CLNR",      "CM",        "CMA",
    "CMAD",      "CMQ",       "CN",        "CNA",       "CNAD",
    "CNQ",       "CY",        "CYB",       "CYP",       "CYR",
    "FIN1PANL1", "FIN1PANL2", "FIN1PANL3", "FIN1PANL4", "FIN2PANL1",
    "FIN2PANL2", "FIN2PANL3", "FIN2PANL4", "XCP"};

static constexpr int NUM_STATES = sizeof(STATES_STR) / sizeof(const char*);
static constexpr int NUM_COEFFS = sizeof(COEFF_STR) / sizeof(const char*);

static constexpr int NITER = 10000;

void getRandomState(const vector<vector<double>>& state_arrays, double* state)
{
    for (size_t i = 0; i < state_arrays.size(); ++i)
    {
        int si   = std::rand() % state_arrays[i].size() - 1;
        double a = state_arrays[i][si];
        double b = state_arrays[i][si + 1];

        state[i] = (b - a) * (std::rand() % 100) / 99 + a;
    }
}

int main()
{
    std::srand(std::time(nullptr));

    AeroData* aerodata = static_cast<AeroData*>(
        initAeroData("for006_states.npz", "for006_coeffs.npz", STATES_STR,
                     NUM_STATES, COEFF_STR, NUM_COEFFS));

    const vector<vector<double>>& state_arrays = aerodata->getStateArrays();

    double* state        = new double[NUM_STATES];
    double* aerodata_vec = new double[NUM_COEFFS];
    const double* cstate = reinterpret_cast<const double*>(state);

    uint64_t dur_coeff = 0;

    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < NITER; ++i)
    {
        getRandomState(state_arrays, state);
        // auto state = chrono::high_resolution_clock::now();
        getAeroCoefficients((void*)aerodata, cstate, NUM_STATES, aerodata_vec,
                            NUM_COEFFS);

        // for(size_t j = 0; j < NUM_COEFFS; ++j)
        // {
        //     cout << aerodata_vec[j] << " ";

        // }
        // cout << "\n";
        // auto dur_getrnd   = state - start;
        // auto dur_getcoeff = end - state;

        // dur_coeff +=
        //     chrono::duration_cast<chrono::microseconds>(dur_getcoeff).count();
    }
    auto end  = chrono::high_resolution_clock::now();
    dur_coeff = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    cout << "Duration: TOTAL: " << dur_coeff / 1000000.0f
         << " ms, single: " << dur_coeff / NITER << " ns\n";
    closeAeroData(aerodata);
    cout << "Done!\n";
}