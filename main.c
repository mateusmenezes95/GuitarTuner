/* 
 * File:   main.c
 * Author: menezes
 *
 * Created on October 12, 2017, 6:58 PM
 */

#include <p33EP128MC204.h>

#include "mcc_generated_files/mcc.h"
#include "guitar_tuner.h"
#include <math.h>

extern fractional pingBuffer[NUM_SAMP];
extern fractional pongBuffer[NUM_SAMP];

fractcomplex signalInComplex[NUM_SAMP]
__attribute__ ((eds, space(ymemory), aligned (NUM_SAMP * 2 *2)));

fractcomplex twiddleFactors[NUM_SAMP/2]
__attribute__ ((section (".xbss, bss, xmemory"), aligned (NUM_SAMP*2)));

fractional hanningWindow[NUM_SAMP];

fractional signalInAbs[NUM_SAMP/2];

extern uint8_t pingBufferFull;
extern uint8_t completedSampling;

int16_t	peakFrequencyBin = 0;
float peakFrequency = 0;
fractional spectrumPower;
fractional averageConstant;
fractional aux_vector[NUM_SAMP];
fractional average_vector[NUM_SAMP];
fractional signal_in_dc_level = 0;

int i = 0;

int main(int argc, char** argv) {
    
    SYSTEM_Initialize();
    AD1CON1bits.ADON = 1;
    
    TwidFactorInit(LOG2_NUM_SAMP, &twiddleFactors[0], 0);
    HanningInit(NUM_SAMP, hanningWindow);
    averageConstant = Float2Fract(0.0019531);
    FillVector(NUM_SAMP, average_vector, averageConstant);
    
    while(1)
    {
        if(completedSampling)
        {
            completedSampling = NO;
            if(pingBufferFull)
            {
                signal_in_dc_level = VectorDotProduct(NUM_SAMP, pingBuffer, average_vector);
                FillVector(NUM_SAMP, aux_vector, signal_in_dc_level);
                VectorSubtract(NUM_SAMP, pingBuffer, pingBuffer, aux_vector);   //Remove dc level from signal in
                for(i = 0; i < NUM_SAMP; i++)
                {
                    signalInComplex[i].real = pingBuffer[i];
                    signalInComplex[i].imag = 0;
                }                
            }
            else
            {
                signal_in_dc_level = VectorDotProduct(NUM_SAMP, pingBuffer, average_vector);
                FillVector(NUM_SAMP, aux_vector, signal_in_dc_level);
                VectorSubtract(NUM_SAMP, pongBuffer, pongBuffer, aux_vector);   //Remove dc level from signal in
                for(i = 0; i < NUM_SAMP; i++)
                {
                    signalInComplex[i].real = pongBuffer[i];
                    signalInComplex[i].imag = 0;
                }                              
            }
            
            VectorWindow(NUM_SAMP, &signalInComplex[0].real, &signalInComplex[0].real, hanningWindow); 
            FFTComplexIP(LOG2_NUM_SAMP, &signalInComplex[0], &twiddleFactors[0], COEFFS_IN_DATA);       //Compute Fast Fourier Transform in place
            BitReverseComplex(LOG2_NUM_SAMP, &signalInComplex[0]);                                    //Organize the vector with complex spectrum
            spectrumPower = VectorPower(NUM_SAMP/2, &signalInComplex[0].real);            
            SquareMagnitudeCplx(NUM_SAMP/2, &signalInComplex[0], signalInAbs);                        //Compute spectrum magnitude and store it another vector
            signalInAbs[0] = 0;
            VectorMax(NUM_SAMP/2, signalInAbs, &peakFrequencyBin);                                    //Find the greatest frequency bin
            GrandkeFreqInterpolation(peakFrequencyBin, signalInAbs, &peakFrequency);
            int x = 0;
            
        }
    }
    return 1;
}

