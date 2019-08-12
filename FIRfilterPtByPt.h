#ifndef  __FIRfilterPtByPt_H__
#define __FIRfilterPtByPt_H__

#include <stdint.h>

/**
* \defgroup  FIRfilterPtByPt  FIR filter point-by-point
* \addtogroup  FIRfilterPtByPt
*
* \brief  Module contains FIR filter structure and functions that implement FIR filter initialization and filtering. Use of filter initialization
*          function is necessary since this function sets internal FIR filter structure fields and zeros out values of circular signal buffer. Circular
*          signal buffer must be longer than number of filter coefficients and must be a power of 2. This buffer must be passed as an argument 
*          to initialization function since FIRfilterObject structure creates it's own pointer to this buffer. Function returns structure, which must be 
*          passed as an argument into a filtering function. Every filter has it's own FIR filter structure. Filtering function applays loop unrolling and
*          computes 8 outputs during one loop iteration in order to provide better performance and speed.
* \{
*/

/**
* \brief   Structure containing variables and pointers which should 
*						be initialized for every FIR filter object only once and must
*           be unchanged during program operation
*/
typedef struct {
		float* signalBuffer;                    /**< Pointer to circular buffer which length must be larger than coefficientsLength and must be a power of 2  */
		uint16_t signalBufferHead;      /**< Index of the newest element of input signal in signalBuffer  */
		uint16_t signalBufferMask;      /**< Bit mask determined from length of signalBuffer  */
		uint16_t signalBufferLength;    /**< Length of circular buffer*/
		uint16_t coefficientsLength;    /**< Length of array with filter coefficients */
		float* coefficients;                     /**< Pointer to array with filter coefficients */
} FIRfilterObject;

FIRfilterObject CreateFIRfilterObject(uint16_t signalBufferLength, float signalCyclicBuffer[], uint16_t coeffLength, float coefficients[]);
void FIRfilterPtByPt(FIRfilterObject* filterObject, float signal, float* result);

/**
* \}
*/

#endif
