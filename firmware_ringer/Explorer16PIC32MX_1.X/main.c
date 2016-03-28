#define SET_CONFIG_FUSES

#include "HardwareProfile.h"
#include <plib.h>


int main(void)
{
    //CORE STUFF
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);    
    SYSTEMConfig(GetSystemClock(), SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    INTEnableSystemMultiVectoredInt();

    
    
    //UART STUFF
    UART_RX_TRIS = 1;
    UART_TX_TRIS = 0;
    UART_RTS_TRIS = 0;
    UART_CTS_TRIS = 1;
    
    //TODO: PPS UNLOCK PROCEDURE
    int intStat, dmaSusp;
    mSYSTEMUnlock(intStat, dmaSusp);
    U1RXR = UART_RX_PPS;
    RPC0R = UART_TX_PPS;
    U1CTSR = UART_CTS_PPS;
    RPB10R = UART_RTS_PPS;
    mSYSTEMLock(intStat, dmaSusp);
    
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
    
    //AG1170 STUFF
    AG1170_RM_LAT = 0;
    AG1170_FR_LAT = 0;
    AG1170_PD_LAT = 0;
    AG1170_RM_TRIS = 0;
    AG1170_FR_TRIS = 0;
    AG1170_PD_TRIS = 0;
    AG1170_SHK_TRIS = 1;
    
    //RN52 STUFF
    RN52_DFU_MODE_LAT = 0;
    RN52_FACTORY_RST_LAT = 0;
    RN52_PWREN_LAT = 0;
    RN52_CMD_MODE_L_LAT = 0;
    RN52_DFU_MODE_TRIS = 0;
    RN52_FACTORY_RST_TRIS = 0;
    RN52_PWREN_TRIS = 0;
    RN52_CMD_MODE_L_TRIS = 0;
    RN52_EVENT_L_TRIS = 1;
            
    while(1);
}

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1Handler(void)
{
    //Toggle the ringer line
    AG1170_FR_LAT ^= 1;
    
    // clear the interrupt flag
    mT1ClearIntFlag();
}

void __ISR(_UART_1_VECTOR, ipl2) Uart1Handler(void)
{
    //TODO: GRAB BYTES
    //QUES: USE DMA INSTEAD?
    
    // clear the interrupt flag
    IFS1bits.U1RXIF = 0;
}