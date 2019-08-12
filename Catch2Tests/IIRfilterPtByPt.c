#include "IIRfilterPtByPt.h"

IIR5thOrderFilterObj IIR_5thOrderSignalFilteringPtByPtInitialize(double forwardCoeff[6], double reverseCoeff[6])
{
    IIR5thOrderFilterObj IIR5thOrderFilterObj_;
    IIR5thOrderFilterObj_.forwardCoeff = forwardCoeff;
    IIR5thOrderFilterObj_.reverseCoeff = reverseCoeff;
    
    return IIR5thOrderFilterObj_;
}

void IIR_5thOrderSignalFilteringPtByPt(IIR5thOrderFilterObj* filterObj, double signal, double* result)
{
    
        double as;

           
                filterObj->stateBuffer[0] += signal * filterObj->forwardCoeff[0];
                filterObj->stateBuffer[1] += signal * filterObj->forwardCoeff[1];
                filterObj->stateBuffer[2] += signal * filterObj->forwardCoeff[2];
                filterObj->stateBuffer[3] += signal * filterObj->forwardCoeff[3];
                filterObj->stateBuffer[4] += signal * filterObj->forwardCoeff[4];
                filterObj->stateBuffer[5] += signal * filterObj->forwardCoeff[5];
            

            as = -filterObj->stateBuffer[0];

                filterObj->stateBuffer[1] += as * filterObj->reverseCoeff[1];
                filterObj->stateBuffer[2] += as * filterObj->reverseCoeff[2];
                filterObj->stateBuffer[3] += as * filterObj->reverseCoeff[3];
                filterObj->stateBuffer[4] += as * filterObj->reverseCoeff[4];
                filterObj->stateBuffer[5] += as * filterObj->reverseCoeff[5];
            

            *result = filterObj->stateBuffer[0];

            filterObj->stateBuffer[0]=filterObj->stateBuffer[1];
            filterObj->stateBuffer[1]=filterObj->stateBuffer[2];
            filterObj->stateBuffer[2]=filterObj->stateBuffer[3];
            filterObj->stateBuffer[3]=filterObj->stateBuffer[4];
            filterObj->stateBuffer[4]=filterObj->stateBuffer[5];
            filterObj->stateBuffer[5]=0;
}
