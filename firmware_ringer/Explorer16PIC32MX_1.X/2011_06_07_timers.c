/** @package 

    timers.c
    
    Copyright(c) Microchip Technology Inc. 2000
    
    Author: ARTHUR ANDERSON
    Created: AA  10/15/2009 10:50:57 AM
	Last change: AA 1/18/2011 2:02:23 PM
*/
/****************************************************************************/
/* Module:  TIMERS.C                                                        */
/*                                                                          */
/* Purpose:                                                                 */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*              S Y S T E M   I N C L U D E   F I L E S                     */
/****************************************************************************/

/****************************************************************************/
/*               L O C A L   I N C L U D E   F I L E S                      */
/****************************************************************************/
#include "HardwareProfile PIC16.h"
#include "timers.h"

/****************************************************************************/
/*                 M A C R O   D E F I N I T I O N S                        */
/****************************************************************************/
//
//

#define TICKS_2MS       (GetSystemClock()/500.0)
#define LATENCY         20
//#define TIMER1_RELOAD   ((int)(-(TICKS_2MS-LATENCY)))
#define TIMER1_RELOAD   (int)(0xFFFF - TICKS_2MS - LATENCY)


/****************************************************************************/
/*                  T Y P E   D E F I N I T I O N S                         */
/****************************************************************************/

/****************************************************************************/
/*        A P P L I C A T I O N   S C O P E   D E C L A R A T I O N S       */
/*                                                                          */
/*  These should be paired with an 'extern' statement in an included header */
/****************************************************************************/


/****************************************************************************/
/*             M O D U L E   S C O P E   D E C L A R A T I O N S            */
/****************************************************************************/
unsigned int Timer_SysTime;
unsigned int now;

/****************************************************************************/
/*                          F U N C T I O N S                               */
/****************************************************************************/

/****************************************************************************/
/* Function:    Constructor                                                 */
/*                                                                          */
/* Initialises the software timer system                                    */
/*                                                                          */
/****************************************************************************/

void Timer_Constructor ( void )
{
    Timer_SysTime = 0;


    //
    TMR1L = 0;                  // prevent rollover to high byte
    TMR1H = (TIMER1_RELOAD >> 8) & 0xff;
    TMR1L = TIMER1_RELOAD & 0xff;
    T1CON = 0b01000001;         // Prescale 1:1, Oscillator off, No synch, intrnal clock. timer ON
    TMR1IE = 1;                 // Enable Timer1 Interrupt
    PEIE = 1;                   // Enable peripheral interrupts
    TMR1IF = 0;                 // but dismiss any spurious interrupts

}
/****************************************************************************/
/* Function:    Timer_Start                                                 */
/*                                                                          */
/* Start a timer of the specified length                                    */
/*                                                                          */
/****************************************************************************/

void _Timer_Start ( Timer_t *theTimer, unsigned int length )
{
    TMR1IE = 0;                 // Disble Timer1 Interrupt
    theTimer->started = Timer_SysTime;
    TMR1IE = 1;                 // Enable Timer1 Interrupt

    length++;
    length /= 2;
    if ( 1==length )
    {
        length++;
    }
    theTimer->length = length;
}

/****************************************************************************/
/* Function:    Timer_IsExpired                                             */
/*                                                                          */
/* Returns TRUE if the provided timer has expired.                          */
/*                                                                          */
/****************************************************************************/

unsigned char _Timer_IsExpired ( Timer_t *theTimer )
{
    int length = theTimer->length;

    if ( length != 0 )
    {
        TMR1IE = 0;                 // Disble Timer1 Interrupt
        now = Timer_SysTime;
        TMR1IE = 1;                 // Enable Timer1 Interrupt

        now -= theTimer->started;
        return (now >= length);
    }

    return 0;
}

/****************************************************************************/
/* Function:    Timer_Stop                                                  */
/*                                                                          */
/* Stops a timer, so that it no longer reports as Expired                   */
/*                                                                          */
/****************************************************************************/
#if 0
// Done as macro
void Timer_Stop ( Timer_t *theTimer )
{
    theTimer->length = 0;
}
#endif
/****************************************************************************/
/* Function:    Timer_IsActive                                              */
/*                                                                          */
/* Returns TRUE if the timer is not stopped.                                */
/* An expired timer will report as active.                                  */
/*                                                                          */
/****************************************************************************/
#if 0
// Done as macro
unsigned char Timer_IsActive ( Timer_t *theTimer )
{
    return theTimer->length != 0;
}
#endif

/****************************************************************************/
/***********************INTERRUPT LEVEL ROUTINE******************************/
/****************************************************************************/
/* Function:    TimerSystem::TickIsr                                        */
/*                                                                          */
/* Handle a tick interrupt from the h/w timer                               */
/*                                                                          */
/****************************************************************************/
/***********************INTERRUPT LEVEL ROUTINE******************************/
/****************************************************************************/

//#pragma inline TimerSystem_TickIsr //why doesn't this work?

void TimerSystem_TickIsr ( void )
{
    TMR1L = 0;                  
    TMR1H = (TIMER1_RELOAD >> 8) & 0xff;
    TMR1L = TIMER1_RELOAD & 0xff;
    TMR1IF = 0; // Re-enable the interrupt

    Timer_SysTime++;
}


