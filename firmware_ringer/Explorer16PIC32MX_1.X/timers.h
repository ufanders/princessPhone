/** @package 

    timers.h
    
    Copyright(c)  2000
    
    Author: ARTHUR ANDERSON
    Created: AA  1/17/2011 2:18:38 PM
    Last Change: AA  1/17/2011 2:18:38 PM
*/
#ifndef TIMERS_H
#define TIMERS_H

/****************************************************************************/
/* Header:  TIMERS.H                                                        */
/*                                                                          */
/* Purpose:                                                                 */
/*      Defines interface to the Timers subsystem                           */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*              S Y S T E M   I N C L U D E   F I L E S                     */
/****************************************************************************/

/****************************************************************************/
/*               L O C A L   I N C L U D E   F I L E S                      */
/****************************************************************************/

/****************************************************************************/
/*                 M A C R O   D E F I N I T I O N S                        */
/****************************************************************************/

/****************************************************************************/
/*                  T Y P E   D E F I N I T I O N S                         */
/****************************************************************************/

//
//  This structure defines a timer and is passed to all functions
//

typedef struct
{
    unsigned int started;
    unsigned int length;
} Timer_t;

/****************************************************************************/
/*        A P P L I C A T I O N   S C O P E   D E C L A R A T I O N S       */
/****************************************************************************/


/****************************************************************************/
/*                          F U N C T I O N S                               */
/****************************************************************************/

//
// Initialise the timer system
//
extern void Timer_Constructor ( void );

extern void TimerSystem_TickIsr(void);

//
//  Start the timer running. Will expire in 'length' milliseconds.
//
#define Timer_Start(t,l) _Timer_Start(&(t), (l))
extern void _Timer_Start ( Timer_t *theTimer, unsigned int length );
//
//  Is the specified timer expired and active?
//  A stopped timer will return false.
//
#define Timer_IsExpired(t) _Timer_IsExpired(&(t))
extern unsigned char _Timer_IsExpired ( Timer_t *theTimer );
//
//  Stop the timer. A stopped timer will not report as expired.
//
//extern void Timer_Stop ( Timer_t *theTimer );
#define Timer_Stop(t) (t).length = 0

//
//  Is a timer active? An expired timer will return TRUE.
//
//extern unsigned char Timer_IsActive ( Timer_t *theTimer );
#define Timer_IsActive(t) ((t).length != 0)
#endif // ndef TIMERS_H

