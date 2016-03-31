//TODO: rotary dial counting
#include "HardwareProfile.h"
#include <plib.h>

/* If off-hook signal is inactive for more than the period of a numeral tick
 * then the rotary counter must reset. Also the phone number aggregator
 * must reset. The RN-52 must exit whatever mode it's in.
*/

enum {
    ROTARY_STATE_ONHOOK,
    ROTARY_STATE_OFFHOOK,
    ROTARY_STATE_COUNTING
};

char rotaryState;
char numberToDial[11]; //includes NULL terminator, necessary?

int rotary_init(void)
{
    rotaryState = ROTARY_STATE_ONHOOK;
    
    INTCONbits.INT4EP = 1; //rising edge sensitive
    INTEnable(INT_EXTERNAL_4_VECTOR, INT_ENABLED);
    
    return 0;
}

int rotary_stateMachine(void)
{
    
    
    return 0;
}

int rotary_numberIsReady(void)
{
    return 0;
}

char* rotary_getNumber(void)
{
    return numberToDial;
}

void __ISR(_EXTERNAL_4_VECTOR, ipl2) Int4Handler(void)
{
    //Stupid, I should have connected this to a pin with INT2 PPS capability.
}
