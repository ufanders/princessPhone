//TODO: bell ringing
#include "HardwareProfile.h"
#include <plib.h>

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
    
    //AG1170 STUFF
    AG1170_RM_LAT = 0; //set mode to normal
    AG1170_FR_LAT = 0; //deassert the ringer line
    AG1170_PD_LAT = 0; //power-on state
    AG1170_RM_TRIS = 0;
    AG1170_FR_TRIS = 0;
    AG1170_PD_TRIS = 0;
    AG1170_SHK_TRIS = 1;
    
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

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{
    //Toggle the ringer line
    AG1170_FR_LAT ^= 1;
    
    // clear the interrupt flag
    mT1ClearIntFlag();
}

