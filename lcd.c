#include "lcd.h"
#include <p33EP128MC204.h>
#include <libpic30.h>
#include "mcc_generated_files/pin_manager.h"

static char mode_lcd;

void LcdSendNibble(char data)
{
    ubyte d;
    d._byte = data;
    
    /*Put four bitS in the output*/
    lcd_d4 = d.bit.b0;
    lcd_d5 = d.bit.b1;
    lcd_d6 = d.bit.b2;
    lcd_d7 = d.bit.b3;
    
    lcd_enable = 1;
    lcd_enable = 0;
}

void LcdSendByte(char address, char data)
{
    lcd_rs = 0;
    lcd_rs = address;
    __delay_ms(1);
    lcd_enable = 0;
    LcdSendNibble(data >> 4);
    LcdSendNibble(data & 0x0F);
}

void LcdInitialize(char mode1, char mode2)
{
    uint8_t i;
    
    lcd_d4 = 0;
    lcd_d5 = 0;
    lcd_d6 = 0;
    lcd_d7 = 0;
    lcd_rs = 0;
    lcd_enable = 0;
    __delay_ms(15);
    
    for(i = 0; i < 3; i++)
    {
        LcdSendNibble(0x03);
        __delay_ms(5);
    }
    
    LcdSendNibble(0x02);
    
    LcdSendByte(COMMAND, LCD_FUNCTION_SET | mode1);
    LcdSendByte(COMMAND, LCD_DISPLAY_CONTROL | mode2);
    
    mode_lcd = LCD_DISPLAY_CONTROL | mode2;
    
    LcdSendByte(COMMAND, LCD_CLEAR_DISPLAY);
    mode_lcd = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
    LcdSendByte(COMMAND, mode_lcd | LCD_ENTRY_MODE_SET);
}

void LcdPlaceText(uint8_t x, uint8_t y)
{
    char address;
    
    if(y)
        address = LCD_SECOND_LINE;
    else
        address = 0;
    
    address += x - 1;
    LcdSendByte(0, LCD_SET_DDRAM_ADDR | address);
}

void LcdPrintChar(uint8_t c)
{
    LcdSendByte(DATA, c);
}

void LcdPrintString(uint8_t *c)
{
    while(*c != '\0')
    {
        LcdSendByte(DATA, *c);
        c++;
    }
}

void LcdClear()
{
    LcdSendByte(COMMAND, LCD_CLEAR_DISPLAY);
    __delay_ms(5);
}

void LcdTurnOn()
{
    mode_lcd |= LCD_DISPLAY_ON;
    LcdSendByte(COMMAND, mode_lcd);
}

void LcdTurnOff()
{
    mode_lcd &= ~LCD_DISPLAY_ON;
    LcdSendByte(COMMAND, mode_lcd);
}

void LcdTurnOnCursor()
{
    mode_lcd |= LCD_CURSOR_ON;
    LcdSendByte(COMMAND, mode_lcd);
}

void LcdTurnOffCursor()
{
    mode_lcd &= ~LCD_CURSOR_ON;
    LcdSendByte(COMMAND, mode_lcd);
}

void LcdTurnOnCursorBlink()
{
    mode_lcd |= LCD_CURSOR_BLINK;
    LcdSendByte(COMMAND, mode_lcd);    
}

void LcdTurnOffCursorBlink()
{
    mode_lcd &= ~LCD_CURSOR_BLINK;
    LcdSendByte(COMMAND, mode_lcd);    
}

void LcdTextRight2Left()
{
    mode_lcd &= ~LCD_ENTRY_LEFT;
    LcdSendByte(COMMAND, LCD_ENTRY_MODE_SET | mode_lcd);
}

void LcdTextLeft2Right()
{
    mode_lcd |= LCD_ENTRY_LEFT;
    LcdSendByte(COMMAND, LCD_ENTRY_MODE_SET | mode_lcd);
}
