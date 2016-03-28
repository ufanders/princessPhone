#include "HardwareProfile.h"
#include <plib.h>

//TODO: bell ringing

//TIMER DEFINES
#define PRESCALE 256
#define TOGGLES_PER_SEC 30
#define T1_TICK (SYS_FREQ/PB_DIV/PRESCALE/TOGGLES_PER_SEC)
#define TIMER_PR_DIVIDEND (SYS_FREQ/PB_DIV/PRESCALE)

/*
 * In North America, the standard ring cadence is "2-4", or two seconds of
 * ringing followed by four seconds of silence. In Australia and the UK, the
 * standard ring cadence is 400 ms on, 200 ms off, 400 ms on, 2000 ms off. These
 * patterns may vary from region to region, and other patterns are used in
 * different countries around the world.
 */

int pots_bellRingInit(char region)
{
    int divisor;
    
    switch(region)
    {
        case 0: //US
            divisor = 20;
            break;
            
        case 1: //UK
            divisor = 25;
            break;
        
        default:
            divisor = 30;
            break;
    }
    
    //RINGING TIMEBASE
    OpenTimer1(T1_OFF | T1_SOURCE_INT | T1_PS_1_256, (TIMER_PR_DIVIDEND/divisor));
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
    
    AG1170_RM_LAT = 0; //set mode to normal
    AG1170_FR_LAT = 0; //deassert the ringer line
    
    return 0;
}

int pots_bellRingStart(void)
{
    AG1170_RM_LAT = 1; //set mode to 'ring'
    
    T1CONbits.ON = 1; //engage timer
    
    return 0;
}

int pots_bellRingStop(void)
{
    T1CONbits.ON = 0;  //disengage timer
    
    AG1170_RM_LAT = 0; //set mode to normal
    
    return 0;
}

int pots_bellRingStatusGet(void)
{
    return AG1170_RM_LAT;
}

//TODO: dial tone

/* 
 * dial tone is a continuous tone having frequencies of 350 and 440 Hz at a level of −13 dBm
 * 
 * http://www.romanblack.com/one_sec.htm#BDA
*/

//TODO: rotary dial counting

/* If off-hook signal is inactive for more than the period of a numeral tick
 * then the rotary counter must reset. Also the phone number aggregator
 * must reset. The RN-52 must exit whatever mode it's in.
*/