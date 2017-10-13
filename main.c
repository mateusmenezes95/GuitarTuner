/* 
 * File:   main.c
 * Author: menezes
 *
 * Created on October 12, 2017, 6:58 PM
 */

#include "mcc_generated_files/mcc.h"


/*
 * 
 */
int main(int argc, char** argv) {
    SYSTEM_Initialize();
    WDT_WatchdogtimerSoftwareDisable();
    TMR1_Start();
    AD1CON1bits.ADON = 1;
    while(1)
    {
        
    }
    return 1;
}

