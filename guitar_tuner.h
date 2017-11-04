/* 
 * File:   guitar_tuner.h
 * Author: menezes
 *
 * Created on October 23, 2017, 3:38 PM
 */

#ifndef GUITAR_TUNER_H
#define	GUITAR_TUNER_H

#include <stdint.h>
#include "dsp.h"


#define NUM_SAMP 512
#define NUM_ZEROS 512
#define LOG2_NUM_SAMP 10
#define SAMPLING_RATE 4000

enum sampleStatus{NO, YES};

/**
  @Summary
    Updates 16-bit timer value

  @Description
    This routine uses the Grandke interpolation frequency algorithm to compute 
 the approximate true peak frequency of a spectrum

  @Param
    peakFreqBin - The bin of greatest frequency of spectrum
    spectrumAbsVector - The vector with the frequency spectrum magnitude
    trueValueFreq - The variable address to store the approximate true peak frequency

  @Returns
    None
 
  @Example 
  
*/
void GrandkeFreqInterpolation (uint16_t peakFreqBin, fractional* spectrumAbsVector, float *trueValueFreq);

/**
  @Summary
    Updates 16-bit timer value

  @Description
    This routine fill a vector with a specified value

  @Param
    vectorSize - Size of vector
    vector2Fill - The vector to fill
    trueValueFreq - Data to be assigned 

  @Returns
    None
 
  @Example 
  
*/

void FillVector(uint16_t vectorSize, fractional *vector2Fill, fractional data);

#endif	/* GUITAR_TUNER_H */

