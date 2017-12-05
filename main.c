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
#include <libpic30.h>
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

uint8_t display_note = 0;
uint8_t play_string_active = YES;

noteFeatures note_in
__attribute__((far,aligned));

int i = 0;

int main(int argc, char** argv) {
    
    SYSTEM_Initialize();
    LcdInitialize(LCD_DISPLAY_8X5 | LCD_2_LINES, LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_CURSOR_FIXED);
    
    LcdClear();
    LcdPlaceText(2,FIRST_LINE);
    LcdPrintString(">GuitarTuner<");

    TwidFactorInit(LOG2_NUM_SAMP, &twiddle_factors[0], 0);
    HanningInit(NUM_SAMP, hanning_window);
    
    while(1)
    {
        TMR1_Tasks_16BitOperation();
        if(TMR1_GetElapsedThenClear())
        {
            if(Button2_GetValue() | Button3_GetValue() | Button4_GetValue())
            {
                display_note ^= 1;
                if(display_note == NO)
                {
                    LcdClear();
                    LcdPlaceText(2,FIRST_LINE);
                    LcdPrintString(">GuitarTuner<");
                }
                else
                {
                    LcdClear();
                    LcdPlaceText(2,FIRST_LINE);
                    LcdPrintString("Toque a corda");
                }
            }
        }
        
        if(completed_sampling)
        {
            completed_sampling = NO;
            RedLed_Toggle(); 
            if(ping_buffer_full)
            {
                VectorWindow(NUM_SAMP, ping_buffer, ping_buffer, hanning_window);
                for(i = 0; i < NUM_SAMP; i++)
                {
                    ping_buffer[i] = ping_buffer[i] >> 1;           
                    signal_in_complex[i].real = ping_buffer[i];
                    signal_in_complex[i].imag = 0;
                }              
            }
            else
            {
                VectorWindow(NUM_SAMP, pong_buffer, pong_buffer, hanning_window);
                for(i = 0; i < NUM_SAMP; i++)
                {
                    pong_buffer[i] = pong_buffer[i] >> 1;
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
            
            if(display_note == YES)
            {
                if(peak_frequency > 67.35 && peak_frequency < 480.05)
                {
                    play_string_active = YES;
                    NoteDetect(peak_frequency, &note_in);
                    ShowNote(&note_in);
                }
                else
                {
                    if(play_string_active)
                    {
                        play_string_active = NO;
                        LcdClear();
                        LcdPlaceText(2,FIRST_LINE);
                        LcdPrintString("Toque a corda");
                    }
                }
            }
        }
//        __delay_ms(100);
    }
    return 1;
}

