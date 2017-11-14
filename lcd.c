#include "lcd.h"
#include <p33EP128MC204.h>
#include <libpic30.h>
#include "mcc_generated_files/pin_manager.h"

static char mode_lcd2;

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
    LcdSendByte(0, 0x20 | mode1);
    LcdSendByte(0, 0x08 | mode2);
    
    mode_lcd2 = 0x08 | mode2;
    
    LcdSendByte(0, 0x01);
    LcdSendByte(0, 0x06);
}

void LcdPlaceText(uint8_t x, uint8_t y)
{
    char address;
    
    if(y)
        address = LCD_SECOND_LINE;
    else
        address = 0;
    
    address += x - 1;
    LcdSendByte(0, 0x80 | address);
}

void LcdWriteChar(uint8_t c)
{
    switch(c)
    {
        case '\f':
            LcdSendByte(0, 0x01);
            __delay_ms(5);
            break;
        case '\n':
//            break;
        case '\r':
            LcdPlaceText(1,2);
            break;
        default:
            LcdSendByte(1, 0x0C);
    }
}

void LcdWriteString(uint8_t *c)
{
    while(*c != '\0')
    {
        LcdSendByte(1, *c);
        c++;
    }
}


