#ifndef  __IIRfilterPtByPt_H__
#define __IIRfilterPtByPt_H__

#include <stdint.h>

/**
* \defgroup  IIRfilterPtByPt  IIR filter point-by-point
* \addtogroup  IIRfilterPtByPt
*
* \brief  Module contains IIR filter structure and functions that implement IIR filter initialization and filtering. Use of filter initialization
*          function is necessary since this function sets internal IIR filter structure fields. Function returns structure, which must be 
*          passed as an argument into a filtering function. Every filter has it's own IIR filter structure. This IIR filter implemantation can be 
*          used only when first reverse coefficient value == 1 
* \warning This IIR filter implemantation can be used only when first reverse coefficient value == 1
* \{
*/

/**
* \brief   Structure containing variables and pointers which should be 
*           initialized for every IIR 2nd order filter object by initialization function 
*           only once and must be unchanged during program operation
*/
typedef struct IIR2ndOrderFilterObj {
    float* forwardCoeff;  /**< Pointer to array with filter forward coefficients */
    float* reverseCoeff;  /**< Pointer to array with filter reverse coefficients */
    float stateBuffer[3];  /**< Buffer with three last filter calculations  */
} IIR2ndOrderFilterObj;

/**
* \brief   Structure containing variables and pointers which should be 
*           initialized for every IIR 3rd order filter object by initialization function 
*           only once and must be unchanged during program operation
*/
typedef struct IIR3rdOrderFilterObj {
    float* forwardCoeff;  /**< Pointer to array with filter forward coefficients */
    float* reverseCoeff;  /**< Pointer to array with filter reverse coefficients */
    float stateBuffer[4];  /**< Buffer with four last filter calculations  */
} IIR3rdOrderFilterObj;

/**
* \brief   Structure containing variables and pointers which should be 
*           initialized for every IIR 4th order filter object by initialization function 
*           only once and must be unchanged during program operation
*/
typedef struct IIR4thOrderFilterObj {
    float* forwardCoeff;  /**< Pointer to array with filter forward coefficients */
    float* reverseCoeff;  /**< Pointer to array with filter reverse coefficients */
    float stateBuffer[5];  /**< Buffer with five last filter calculations  */
} IIR4thOrderFilterObj;

IIR2ndOrderFilterObj  IIR_2ndOrderSignalFilteringPtByPtInitialize(float forwardCoeff[3], float reverseCoeff[3]);
IIR3rdOrderFilterObj  IIR_3rdOrderSignalFilteringPtByPtInitialize(float forwardCoeff[4], float reverseCoeff[4]);
IIR4thOrderFilterObj  IIR_4thOrderSignalFilteringPtByPtInitialize(float forwardCoeff[5], float reverseCoeff[5]);

void IIR_2ndOrderSignalFilteringPtByPt(IIR2ndOrderFilterObj* filterObject, float signal, float* result);
void IIR_3rdOrderSignalFilteringPtByPt(IIR3rdOrderFilterObj* filterObject, float signal, float* result);
void IIR_4thOrderSignalFilteringPtByPt(IIR4thOrderFilterObj* filterObject, float signal, float* result);

/**
* \}
*/
#endif
