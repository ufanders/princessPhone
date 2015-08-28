/*********************************************************************
 *
 *      Example Binky LEDs
 *
 *********************************************************************
 * FileName:        main.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *********************************************************************
 * This program uses Explorer-16 to blink all of its LEDs at once.
 *
 * Platform: 	Explorer-16 with PIC32MX PIM
 *
 ********************************************************************/
#include <plib.h>

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ 			(80000000L)
#define PB_DIV                 8
#define PRESCALE               256
#define TOGGLES_PER_SEC        30
#define T1_TICK               (SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)

int main(void)
{
    int i;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);


    // Explorer16 LEDs are on lower 8-bits of PORTA and to use all LEDs, JTAG port must be disabled.
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

    // Make all lower 8-bits of PORTA as output. Turn them off before changing
    // direction so that we don't have unexpected flashes
    mPORTAClearBits(BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
                                                     BIT_3 | BIT_2 | BIT_1 | BIT_0 );

    mPORTASetPinsDigitalOut( BIT_7 | BIT_6 | BIT_5 | BIT_5 | BIT_4 | \
                                                     BIT_3 | BIT_2 | BIT_1 | BIT_0 );

// STEP 2. configure Timer 1 using internal clock, 1:256 prescale

    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, T1_TICK);

    // set up the timer interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);

    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    //set the ringer mode
    mPORTASetBits(BIT_6);
    
    //clear the ringer line
    mPORTAClearBits(BIT_7);


    // Now blink all LEDs ON/OFF forever.
    while(1);
}

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{
    // clear the interrupt flag
    mT1ClearIntFlag();

    //Toggle the ringer line
    mPORTAToggleBits(BIT_7);
}
