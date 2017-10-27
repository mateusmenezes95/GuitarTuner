/* 
 * File:   main.c
 * Author: menezes
 *
 * Created on October 12, 2017, 6:58 PM
 */

#include <p33EP128MC204.h>

#include "mcc_generated_files/mcc.h"
#include "guitar_tuner.h"
#include "dsp.h"

extern fractional pingBuffer[NUM_SAMP];

extern fractional pongBuffer[NUM_SAMP];

fractcomplex signalIn[NUM_SAMP]
__attribute__ ((eds, space(ymemory), aligned (NUM_SAMP * 2 *2)));

fractional signaInAbs[NUM_SAMP/2];

extern uint8_t pingBufferFull;
extern uint8_t completedSampling;

int16_t	peakFrequencyBin = 0;
uint32_t peakFrequency = 0;

int i = 0;

fractcomplex twiddleFactors[NUM_SAMP/2]
__attribute__ ((section (".xbss, bss, xmemory"), aligned (NUM_SAMP*2)));

/*
 * 
 */
int main(int argc, char** argv) {
    
    SYSTEM_Initialize();
    AD1CON1bits.ADON = 1;
    
    TwidFactorInit(LOG2_NUM_SAMP, &twiddleFactors[0], 0);
    
    while(1)
    {
        if(completedSampling)
        {
            completedSampling = NO;
            if(pingBufferFull)
            {
                for(i = 0; i < NUM_SAMP; i++)
                {
                    signalIn[i].real = pingBuffer[i] >> 1;
                    signalIn[i].imag = 0;
                }
            }
            else
            {
                for(i = 0; i < NUM_SAMP; i++)
                {
                    signalIn[i].real = pongBuffer[i] >> 1;
                    signalIn[i].imag = 0;
                }
            }
            FFTComplexIP(LOG2_NUM_SAMP, &signalIn[0], &twiddleFactors[0], COEFFS_IN_DATA);
            BitReverseComplex(LOG2_NUM_SAMP, &signalIn[0]);
            SquareMagnitudeCplx(NUM_SAMP/2, &signalIn[0], signaInAbs);
//            signaInAbs[0]= 0;
            VectorMax(NUM_SAMP/2, signaInAbs, &peakFrequencyBin);
            peakFrequency = peakFrequencyBin*(SAMPLING_RATE/NUM_SAMP);
            
        }
    }
    return 1;
}

