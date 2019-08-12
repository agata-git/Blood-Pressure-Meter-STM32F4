#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "FIRfilterPtByPt.c"
#include "FilteredSignal250Hz.h"
#include "InputSignal250Hz.h"
#include "fdacoefs.h"
#include <cmath>

extern double InputSignal250Hz[];
extern double FilteredSignal250Hz[];
extern double FIRlowpass8HzCoeff[54];
extern const int FIRlowpass8HzLength;

unsigned int coeffLength = FIRlowpass8HzLength;
double y[128];
FIRfilterObject FIRhighpassFilter;
FIRfilterObject* FIRhighpassFilterObject;



TEST_CASE("FIR_FilteringPtByPt_function_test", "[FIRsignalPtByPTFiltering]")
{
    FIRhighpassFilterObject = &FIRhighpassFilter;
    double filtered[1000];
    
    FIRhighpassFilter = CreateFIRfilterObject(128, y, coeffLength, FIRlowpass8HzCoeff);
    
    for(int cnt=0; cnt<1000; cnt++)
    {
        FIRfilterPtByPt(FIRhighpassFilterObject,InputSignal250Hz[cnt], &filtered[cnt]);
        REQUIRE(fabs((filtered[cnt])-(FilteredSignal250Hz[cnt]))<=0.000000001);
    }
}
