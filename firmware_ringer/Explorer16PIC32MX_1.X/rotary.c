//TODO: rotary dial counting
#include "HardwareProfile.h"
#include <plib.h>

/* If off-hook signal is inactive for more than the period of a numeral tick
 * then the rotary counter must reset. Also the phone number aggregator
 * must reset. The RN-52 must exit whatever mode it's in.
*/

//#define THRESHOLD_NUMERAL_MARK ?
#define THRESHOLD_NUMERAL_SPACE 1

enum {
    ROTARY_STATE_ONHOOK,
    ROTARY_STATE_OFFHOOK,
    ROTARY_STATE_COUNTING
};

unsigned int timerTicks;

char rotaryState, numeralMarkExpired, numeralSpaceExpired, numeralTickTock, numeralValue;
char numberIsReady, numeralIndex, numberToDial[11]; //includes NULL terminator, necessary?

int rotary_init(void)
{
    timerTicks = 0;
    numeralMarkExpired = 0;
    numeralSpaceExpired = 0;
    
    numeralTickTock = 0;
    numeralValue = 0;
    numeralIndex = 0;
    numberIsReady = 0;
    
    if(AG1170_SHK_PORT)
    {
        rotaryState = ROTARY_STATE_OFFHOOK;
        INTCONbits.INT4EP = 0; //falling edge sensitive
    }
    else 
    {
        rotaryState = ROTARY_STATE_ONHOOK;
        INTCONbits.INT4EP = 1; //rising edge sensitive
    }
    
    INTEnable(INT_EXTERNAL_4_VECTOR, INT_ENABLED);
    
    OpenTimer3(T3_OFF | T3_SOURCE_INT | T3_PS_1_256, (GetPeripheralClock()/256/4)); //250ms timer
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_2);
    
    return 0;
}

int rotary_stateMachine(void)
{
    switch(rotaryState)
    {
        case ROTARY_STATE_ONHOOK:
             if(AG1170_SHK_PORT) rotaryState = ROTARY_STATE_COUNTING;
            break;
            
        case ROTARY_STATE_COUNTING:
            if(!AG1170_SHK_PORT && numeralSpaceExpired)
            {
                //reset everything, we've gone on-hook.
                T3CONbits.ON = 0;
            }
            break;
            
            if(!numeralIndex && (numeralTickTock == 0b01))
            {
                //this is the first falling edge - start the space timer.
                TMR3 = 0;
                timerTicks = 0;
                T3CONbits.ON = 1;
            }
            break;
            
            if(AG1170_SHK_PORT && numeralSpaceExpired)
            {
                //TODO: save numeral and scoot to next one
                numberToDial[numeralIndex++] = numeralValue;
                numeralValue = 0;
                
                if(numeralIndex == 11) //we have achieved a number to dial!
                {
                    T3CONbits.ON = 0; //stop space timer

                    numberIsReady = 1;
                    numeralIndex = 0;
                }
            }
            break;
            
        default:
            break;
    }

    return 0;
}

int rotary_numberIsReady(void)
{
    return numberIsReady;
}

char* rotary_getNumber(void)
{
    numberIsReady = 0;
    
    return numberToDial;
}

void __ISR(_EXTERNAL_4_VECTOR, ipl2) Int4Handler(void)
{
    //Stupid, I should have connected this to a pin with INT2 PPS capability.
    
    //if falling edge is detected, start tick-slot timer.
    //if falling edge lasts past tick-slot timer, reset.
    //within this tick-slot time, every rising edge increments numeral.
    //count falling/rising edges found within tick-slot timer
    
    if(!AG1170_SHK_PORT)
    {
        numeralTickTock |= 0b01;
        INTCONbits.INT4EP = 1; //rising edge sensitive
    }
    else 
    {
        numeralTickTock |= 0b10;
        INTCONbits.INT4EP = 0; //falling edge sensitive
    }
    
    if(numeralTickTock == 0b11)
    {
        numeralValue++;
        numeralTickTock = 0;
    }
    
    TMR3 = 0; //clear timer period register
    timerTicks = 0; //reset space timer
    
    IFS0bits.INT4IF = 0; //clear int flag
}

void __ISR(_TIMER_3_VECTOR, ipl2) Timer3Handler(void)
{
    timerTicks++;
    
    if(timerTicks == THRESHOLD_NUMERAL_SPACE) 
    {
        numeralSpaceExpired = 1;
        timerTicks = 0;
    }
    
    mT3ClearIntFlag(); //clear the interrupt flag
}