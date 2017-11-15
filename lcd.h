/* 
 * File:   lcd.h
 * Author: menezes
 *
 * Created on November 13, 2017, 7:43 PM
 */

#ifndef LCD_H
#define	LCD_H

#include <stdint.h>

#define FCY 140000000

/* Pins connected to display*/
#define lcd_enable LATBbits.LATB1
#define lcd_rs LATBbits.LATB2
#define lcd_d4 LATBbits.LATB3
#define lcd_d5 LATCbits.LATC0
#define lcd_d6 LATCbits.LATC1
#define lcd_d7 LATCbits.LATC2

#define LCD_SECOND_LINE 0x40
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_CURSOR_BLINK 0x01
#define LCD_CURSOR_FIXED 0x00
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_DISPLAY_8X5 0x00
#define LCD_DISPLAY_10X5 0x04
#define LCD_2_LINES 0x08
#define LCD_1_LINE 0x00

typedef union
{
    char _byte;
    struct
    {
        char b0 : 1;
        char b1 : 1;
        char b2 : 1;
        char b3 : 1;
        char b4 : 1;
        char b5 : 1;
        char b6 : 1;
        char b7 : 1;
    }bit;
}ubyte;


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
void LcdSendNibble(char data);

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
void LcdSendByte(char address, char data);

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
void LcdInitialize(char mode1, char mode2);

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

void LcdPlaceText(uint8_t x, uint8_t y);

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
void LcdWriteChar(uint8_t c);

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
void LcdWriteString(uint8_t *c);

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
void LcdTurnOn(void);

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
void LcdTurnOff(void);

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
void LcdTurnOnCursor(void);

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
void LcdTurnOffCursor(void);

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
void LcdTurnOnCursorBlink(void);

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
void LcdTurnOffCursorBlink(void);

#endif	/* LCD_H */

