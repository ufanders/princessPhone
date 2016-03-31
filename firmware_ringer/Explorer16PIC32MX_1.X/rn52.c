//TODO: Bluetooth

#include "HardwareProfile.h"
#include <plib.h>

char response[255], currentResponseIndex, responseComplete;

int rn52_init(void)
{
    //TODO: PPS UNLOCK PROCEDURE
    int intStat, dmaSusp;
    mSYSTEMUnlock(intStat, dmaSusp);
    U1RXR = UART_RX_PPS;
    RPC0R = UART_TX_PPS;
    U1CTSR = UART_CTS_PPS;
    RPB10R = UART_RTS_PPS;
    mSYSTEMLock(intStat, dmaSusp);    
    
    //UART STUFF
    UART_RX_TRIS = 1;
    UART_TX_TRIS = 0;
    UART_RTS_TRIS = 0;
    UART_CTS_TRIS = 1;
    
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
    
    INTCONbits.INT3EP = 1; //rising edge sensitive?
    INTEnable(INT_EXTERNAL_3_VECTOR, INT_ENABLED);
    
    currentResponseIndex = 0;
    responseComplete = 0;
    
    return 0;
}

int rn52_getStatus(void)
{
    puts("Q\r");

    return rn52_getErr();
}

int rn52_getErr(void)
{
    return 0;
}

int rn52_dial(char* str)
{

    return 0;
}

int rn52_accept(void)
{
    return 0;
}

int rn52_reject(void)
{
    return 0;
}

int rn52_pairStart(void)
{
    return 0;
}

int rn52_pairStop(void)
{
    return 0;
}

void __ISR(_EXTERNAL_3_VECTOR, ipl2) Int3Handler(void)
{
    //Stupid, I should have connected this to a pin with INT1 PPS capability.
}

void __ISR(_UART_1_VECTOR, ipl2) Uart1Handler(void)
{
    //TODO: GRAB BYTES
    //QUES: USE DMA INSTEAD?
    
    response[currentResponseIndex] = U1RXREG;
    
    // clear the interrupt flag
    IFS1bits.U1RXIF = 0;
}