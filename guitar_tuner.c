/* 
 * File:   guitar_tuner.c
 * Author: menezes
 *
 * Created on October 29, 2017, 12:32 PM
 */

#include "guitar_tuner.h"

float guitar_notes[NUM_NOTES] = 
{
    65.41,      //0-do2      
    69.30,      //1-do#2
    73.42,      //2-re2
    77.79,      //3-re#2
    82.41,      //4-mi2
    87.31,      //5-fa2
    92.50,      //6-fa#2
    98.00,      //7-sol2
    103.83,     //8-sol#2
    110.01,     //9-la2
    116.55,     //10-la#2
    123.48,     //11-si2
    130.82,     //12-do3
    138.60,     //13-do#3
    146.84,     //14-re3
    155.57,     //15-re#3
    164.82,     //16-mi3
    174.62,     //17-fa3
    185.01,     //18-fa#3
    196.01,     //19-sol3
    207.66,     //20-sol#3
    220.01,     //21-la3
    233.09,     //22-la#3
    246.96,     //23-si3
    261.64,     //24-do4
    277.20,     //25-do#4
    293.68,     //26-re4
    311.14,     //27-re#4
    329.65,     //28-mi4
    349.25,     //29-fa4
    370.01,     //30-fa#4
    392.02,     //31-sol4
    415.33,     //32-sol#4    
    440.02,     //33-la4
    466.19,     //34-la#4
    493.91      //35-si4
};

/*
 * 
 */
void GrandkeFreqInterpolation(uint16_t peakFreqBin, fractional *spectrumAbsVector, float *trueValueFreq)
{
    float delta;
    float alfa;
    float resolution = ((float)SAMPLING_RATE / (NUM_SAMP+NUM_ZEROS));
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

void FillVector(uint16_t vectorSize, fractional *vector2Fill, fractional data)
{
    int i;
    
    for(i = 0; i < vectorSize; i++)
    {
        vector2Fill[i] = data;
    }
}

void NoteDetect(float freq_value, noteFeatures *note)
{
    float freq_ref = guitar_notes[0];
    uint8_t i;
    float i_fractional;
    float up_limit;
    float down_limit;
    float a, b, c;
    
    i_fractional = ((12.0 * log10(freq_value / freq_ref)) / 0.301030);
    i = (uint8_t)i_fractional;
    i_fractional = i_fractional - (float)i;
    
    if(i_fractional >= 0.49278)
        i += 1;
    
    note->note_index = i;
    
    up_limit = (guitar_notes[i] + guitar_notes[i+1]) * 0.5;
    down_limit = (guitar_notes[i] + guitar_notes[i-1]) * 0.5;
    
    if(freq_value >= down_limit && freq_value < guitar_notes[i])
    {
        note->tuning = BELOW;
        b = (down_limit + guitar_notes[i]) * 0.5;
        a = (down_limit + b) * 0.5;
        c = (b + guitar_notes[i]) * 0.5;
    }
    else
    {
        note->tuning = ABOVE;
        b = (guitar_notes[i] + up_limit) * 0.5;
        a = (guitar_notes[i] + b) * 0.5;
        c = (b + up_limit) * 0.5;
    }
    
    if(note->tuning == BELOW)
    {
        if(freq_value >= down_limit && freq_value <= a)
            note->tuning_interval = D;
        else if(freq_value > a && freq_value <= b)
            note->tuning_interval = C;
        else if(freq_value > b && freq_value <= c)
            note->tuning_interval = B;
        else if(freq_value > c && freq_value <= guitar_notes[i])
            note->tuning_interval = A;
    }
    else
    {
        if(freq_value >= guitar_notes[i] && freq_value < a)
            note->tuning_interval = A;
        else if(freq_value >= a && freq_value < b)
            note->tuning_interval = B;
        else if(freq_value >= b && freq_value < c)
            note->tuning_interval = C;
        else if(freq_value >= c && freq_value <= up_limit)
            note->tuning_interval = D;
    }
    int x = 0;
}

void ShowNote(noteFeatures *note)
{
    switch(note->tuning)
    {
        case BELOW:
//            BlueLed_SetLow();
            break;
        case ABOVE:
//            BlueLed_SetHigh();
            break;
    }
    
    
    switch(note->tuning_interval)
    {
        case A:
//            WhiteLed_SetHigh();
//            GreenLed_SetLow();
//            YellowLed_SetLow();
//            RedLed_SetLow();
            break;
        case B:
//            WhiteLed_SetLow();
//            GreenLed_SetHigh();
//            YellowLed_SetLow();
//            RedLed_SetLow();
            break;
        case C:
//            WhiteLed_SetLow();
//            GreenLed_SetLow();
//            YellowLed_SetHigh();
//            RedLed_SetLow();
            break;
        case D:
//            WhiteLed_SetLow();
//            GreenLed_SetLow();
//            YellowLed_SetLow();
//            RedLed_SetHigh();
            break;
    }
}

