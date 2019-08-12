#include <stdlib.h>

typedef struct IIR5thOrderFilterObj {
    double* forwardCoeff;
    double* reverseCoeff;
    double stateBuffer[6];
} IIR5thOrderFilterObj;

IIR5thOrderFilterObj IIR_5thOrderSignalFilteringPtByPtInitialize(double forwardCoeff[6], double reverseCoeff[6]);
void IIR_5thOrderSignalFilteringPtByPt(IIR5thOrderFilterObj* filterObj, double signal, double* result);

