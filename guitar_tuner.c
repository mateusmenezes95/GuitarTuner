/* 
 * File:   guitar_tuner.c
 * Author: menezes
 *
 * Created on October 29, 2017, 12:32 PM
 */

#include "guitar_tuner.h"

/*
 * 
 */
void grandkeFreqInterpolation(uint16_t peakFreqBin, fractional *spectrumAbsVector, float *trueValueFreq)
{
    float delta;
    float alfa;
    float resolution = ((float)SAMPLING_RATE / NUM_SAMP);
    uint16_t secondPeakFreqBin;
    uint8_t secondPeakBinAfter;
    
    if(spectrumAbsVector[peakFreqBin + 1] > spectrumAbsVector[peakFreqBin - 1])
    {
        secondPeakFreqBin = peakFreqBin + 1;
        secondPeakBinAfter = YES;
    }
    else
    {
        secondPeakFreqBin = peakFreqBin - 1;
        secondPeakBinAfter = NO;
    }
    
    alfa = (float)secondPeakFreqBin / (float)peakFreqBin;
    
    delta = ((2*alfa) - 1) / (alfa+1);
//    int x = 0;
    if(secondPeakBinAfter)
        *trueValueFreq = ((float)peakFreqBin + delta) * resolution;
    else
        *trueValueFreq = ((float)peakFreqBin - delta) * resolution;        
}

