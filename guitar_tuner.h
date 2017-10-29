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
#define LOG2_NUM_SAMP 9
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
void grandkeFreqInterpolation (uint16_t peakFreqBin, fractional* spectrumAbsVector, float *trueValueFreq);

#endif	/* GUITAR_TUNER_H */

