#include "AeroData.h"
#include "ModelicaAeroData.h"

#include <iostream>

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



int main()
{
    AeroData* aerodata = static_cast<AeroData*>(
        initAeroData("for006_states.npz", "for006_coeffs.npz", STATES_STR,
                     NUM_STATES, COEFF_STR, NUM_COEFFS));
    
    // for()

    closeAeroData(aerodata);
    cout << "Done!\n";
}