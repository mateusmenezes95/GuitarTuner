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
#include "lcd.h"

extern fractional ping_buffer[NUM_SAMP];
extern fractional pong_buffer[NUM_SAMP];
extern float guitar_notes[NUM_NOTES];

fractcomplex signal_in_complex[NUM_SAMP+NUM_ZEROS]
__attribute__ ((eds, space(ymemory), aligned ((NUM_SAMP+NUM_ZEROS) * 2 *2)));

fractcomplex twiddle_factors[(NUM_SAMP+NUM_ZEROS)/2]
__attribute__ ((section (".xbss, bss, xmemory"), aligned ((NUM_SAMP+NUM_ZEROS)*2)));

fractional hanning_window[NUM_SAMP];

fractional signal_in_Abs[(NUM_SAMP+NUM_ZEROS)/2];

extern uint8_t ping_buffer_full;
extern uint8_t completed_sampling;

int16_t	peak_frequency_bin = 0;
float peak_frequency = 0;
fractional spectrum_power;
fractional average_constant;
fractional aux_vector[NUM_SAMP];
fractional average_vector[NUM_SAMP];
fractional signal_in_dc_level = 0;
noteFeatures note_in
__attribute__((far,aligned));

int i = 0;

int main(int argc, char** argv) {
    
    SYSTEM_Initialize();
    LcdInitialize(LCD_DISPLAY_8X5 | LCD_2_LINES, LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_CURSOR_FIXED);
    LcdWriteChar('\f');
    LcdPlaceText(3,0);
    char t[] = "GuitarTuner"; 
//    LcdSendByte(1,t);
    LcdWriteString(t);
    LcdPlaceText(4,1);
    LcdWriteString("Funfando!");
    AD1CON1bits.ADON = 1;
    
    TwidFactorInit(LOG2_NUM_SAMP, &twiddle_factors[0], 0);
    HanningInit(NUM_SAMP, hanning_window);
    average_constant = Float2Fract(1.0/NUM_SAMP);
    FillVector(NUM_SAMP, average_vector, average_constant);
    
    while(1)
    {
        if(completed_sampling)
        {
            completed_sampling = NO;
            if(ping_buffer_full)
            {
                signal_in_dc_level = VectorDotProduct(NUM_SAMP, ping_buffer, average_vector);
                FillVector(NUM_SAMP, aux_vector, signal_in_dc_level);
                VectorSubtract(NUM_SAMP, ping_buffer, ping_buffer, aux_vector);   //Remove dc level from signal in
                VectorWindow(NUM_SAMP, ping_buffer, ping_buffer, hanning_window);
                for(i = 0; i < NUM_SAMP; i++)
                {
                    ping_buffer[i] = ping_buffer[i] << 3;           
                    signal_in_complex[i].real = ping_buffer[i];
                    signal_in_complex[i].imag = 0;
                }              
            }
            else
            {
                signal_in_dc_level = VectorDotProduct(NUM_SAMP, pong_buffer, average_vector);
                FillVector(NUM_SAMP, aux_vector, signal_in_dc_level);
                VectorSubtract(NUM_SAMP, pong_buffer, pong_buffer, aux_vector);   //Remove dc level from signal in
                VectorWindow(NUM_SAMP, pong_buffer, pong_buffer, hanning_window);
                for(i = 0; i < NUM_SAMP; i++)
                {
                    pong_buffer[i] = pong_buffer[i] << 3;
                    signal_in_complex[i].real = pong_buffer[i];
                    signal_in_complex[i].imag = 0;
                }                              
            }
            VectorZeroPad(NUM_SAMP, NUM_ZEROS, &signal_in_complex[0].real, &signal_in_complex[0].real);
            VectorZeroPad(NUM_SAMP, NUM_ZEROS, &signal_in_complex[0].imag, &signal_in_complex[0].imag);
            FFTComplexIP(LOG2_NUM_SAMP, &signal_in_complex[0], &twiddle_factors[0], COEFFS_IN_DATA);       
            BitReverseComplex(LOG2_NUM_SAMP, &signal_in_complex[0]);                                               
            SquareMagnitudeCplx((NUM_SAMP+NUM_ZEROS)/2, &signal_in_complex[0], signal_in_Abs);                        
            signal_in_Abs[0] = 0;
            VectorMax((NUM_SAMP+NUM_ZEROS)/2, signal_in_Abs, &peak_frequency_bin);                                    
            GrandkeFreqInterpolation(peak_frequency_bin, signal_in_Abs, &peak_frequency);
            
            if(peak_frequency < 500.0)
            {
//                TMR1_Start();
                NoteDetect(peak_frequency, &note_in);
<<<<<<< HEAD
                ShowNote(&note_in);
            }            
            int x = 0;  
=======
//                ShowNote(&note_in);
            }
            else
            {
                TMR1_Stop();
            }
            
            int x = 0;
            
>>>>>>> 8b4e1691fafdbfbe17c7852018703359c1934171
        }
    }
    return 1;
}

