#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "IIRfilterPtByPt.c"
#include "waveforms.cpp"
#include <cmath>

extern double InputSignal_f32_1kHz_15kHz[320];
extern double InputSignal_f32_1kHz_15kHz_IIR_filtered[320];
extern double ForwardCoefficients[6];
extern double ReverseCoefficients[6];

IIR5thOrderFilterObj IIRfilterObj_;
IIR5thOrderFilterObj* IIRfilterObj;

TEST_CASE("IIR_6thOrderSignalFilteringPtByPt_function_test", "[IIRsignalPtByPTFiltering]")
{
    double filtered[320];
    IIRfilterObj_ = IIR_5thOrderSignalFilteringPtByPtInitialize(ForwardCoefficients, ReverseCoefficients);
    IIRfilterObj = &IIRfilterObj_;
    
    for(int cnt=0; cnt<320; cnt++)
    {
        IIR_5thOrderSignalFilteringPtByPt(IIRfilterObj, InputSignal_f32_1kHz_15kHz[cnt], &filtered[cnt]);
        REQUIRE(fabs((filtered[cnt])-(InputSignal_f32_1kHz_15kHz_IIR_filtered[cnt]))<=0.0001);
    }
}
