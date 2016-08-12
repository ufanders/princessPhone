#define SET_CONFIG_FUSES

#include "HardwareProfile.h"
#include <plib.h>

unsigned char status;

int main(void)
{
    //CORE STUFF
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);    
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    INTEnableSystemMultiVectoredInt();
    //INTEnableInterrupts();
    
    //BUTTON STUFF
    SW0_TRIS = 1;
    //SW1_TRIS = 1; //RB6 DOESN'T EXIST ON '270
    SW2_TRIS = 1;
    
    //LED STUFF
    LED_RED_LAT = 0;
    LED_GRN_LAT = 0;
    LED_RGB_RED_LAT = 0;
    LED_RGB_GRN_LAT = 0;
    LED_RGB_BLU_LAT = 0;
    LED_RED_TRIS = 0;
    LED_GRN_TRIS = 0;
    LED_RGB_RED_TRIS = 0;
    LED_RGB_GRN_TRIS = 0;
    LED_RGB_BLU_TRIS = 0;
    
    status = 0;
    
    status = rn52_init();
    status = dtmf_init();
    status = ag1170_init(0);
    status = rotary_init();
    
    while(status); //halt here if any init function returns an error
    
    while(1)
    {
        princessPhone_tasks();
    }
}
