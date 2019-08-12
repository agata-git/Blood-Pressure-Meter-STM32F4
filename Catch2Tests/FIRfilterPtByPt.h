#ifndef  __FIRfilterPtByPt_H__
#define __FIRfilterPtByPt_H__

#include <stdint.h>

/**
* \defgroup  FIRfilterPtByPt  FIR filter point-by-point
* \addtogroup  FIRfilterPtByPt
*
* \brief  Module contains FIR filter structure and functions that implement FIR filter initialization and filtering
* \{
*/

/**
* \brief   Structure containing variables and pointers which should 
*						be initialized for every FIR filter object only once and must
*           be unchanged during program operation
*/
typedef struct {
		double* signalBuffer;               /**< Pointer to circular buffer which length must be larger than coefficientsLength and must be a power of 2  */
		unsigned int signalBufferHead;      /**< Index of the newest element of input signal in signalBuffer  */
		unsigned int signalBufferMask;      /**< Bit mask determined from length of signalBuffer  */
		unsigned int signalBufferLength;    /**< Length of circular buffer*/
		unsigned int coefficientsLength;    /**< Length of array with filter coefficients */
		double* coefficients;               /**< Pointer to array with filter coefficients */
} FIRfilterObject;

FIRfilterObject CreateFIRfilterObject(unsigned int signalBufferLength, double signalCyclicBuffer[], unsigned int coeffLength, double coefficients[]);
void FIRfilterPtByPt(FIRfilterObject* filterObject, double signal, double* result);

#endif
