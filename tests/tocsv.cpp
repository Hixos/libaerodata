#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "AeroData.h"
#include "ModelicaAeroData.h"

using namespace std;

using vd = vector<double>;

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

template <typename T>
void printCSV(ofstream& f, const vector<T>& vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
    {
        f << vec[i];
        if (i != vec.size() - 1)
        {
            f << ",";
        }
    }
    f << "\n";
}

int main()
{
    static const int NUM_STATES = sizeof(STATES_STR) / sizeof(double);
    static const int NUM_COEFFS = sizeof(COEFF_STR) / sizeof(double);

    AeroData* aerodata = static_cast<AeroData*>(
        initAeroData("result_states.npz", "result_coeffs.npz", STATES_STR,
                     NUM_STATES, COEFF_STR, NUM_COEFFS));

    vector<string> keys(std::begin(STATES_STR), std::end(STATES_STR));
    keys.insert(keys.end(), std::begin(COEFF_STR), std::end(COEFF_STR));

    ofstream csv("aerodata.csv");
    csv.precision(6);
    // csv << std::scientific;

    printCSV(csv, keys);

    size_t size = aerodata->getTensorFlatSize();
    for (size_t i = 0; i < size; ++i)
    {
        vector<double> state = aerodata->getState(i);
        vector<double> data = aerodata->getAeroData(state);

        vector<double> row(state);
        row.insert(row.end(), data.begin(), data.end());
        printCSV(csv, row);
    }

    closeAeroData(aerodata);
    cout << "Done!\n";
    return 0;
}