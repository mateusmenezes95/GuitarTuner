/*******************************************************************************
  DMA Generated Driver File

Company:
Microchip Technology Inc.

File Name:
    dma.c

Summary:
This is the generated driver implementation file for the DMA driver using PIC24 / dsPIC33 / PIC32MM MCUs

Description:
This source file provides implementations for driver APIs for DMA.
Generation Information :
Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
Device            :  dsPIC33EP128MC204
The generated drivers are tested against the following:
Compiler          :  XC16 1.31
MPLAB             :  MPLAB X 3.60
*******************************************************************************/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <xc.h>
#include "dma.h"
#include "../guitar_tuner.h"

fractional ping_buffer[NUM_SAMP];

fractional pong_buffer[NUM_SAMP];

uint8_t ping_buffer_full = 0;
uint8_t completed_sampling = 0;

void DMA_Initialize(void) 
{ 
	DMA0CONbits.AMODE = 0;			// Configure DMA for Register indirect with post increment
	DMA0CONbits.MODE  = 2;			// Configure DMA for Continuous Ping-Pong mode
    
    DMA0REQ = 0xD;
    DMA0CNT = NUM_SAMP - 1;
    DMA0PAD = (volatile unsigned int)&ADC1BUF0;
    
	DMA0STAL = __builtin_dmaoffset(ping_buffer);	
    DMA0STAH = 0x0000;
    
	DMA0STBL = __builtin_dmaoffset(pong_buffer);
    DMA0STBH = 0x0000;
    
    IFS0bits.DMA0IF = false;    // Clearing Channel 0 Interrupt Flag;
    IEC0bits.DMA0IE = 1;        // Enabling Channel 0 Interrupt
    
    DMA0CONbits.CHEN = 1;       //Enable DMA Channel 0
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _DMA0Interrupt( void )
{
    completed_sampling = YES;
    ping_buffer_full ^= 1;
    IFS0bits.DMA0IF = 0;
}

/**
  End of File
*/
