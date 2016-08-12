//TODO: Bluetooth

#include "HardwareProfile.h"
#include <plib.h>

char response[255], currentResponseIndex, responseComplete, statusChanged;
int status;
char responseAOK[] = "AOK\r\n";
char responseERR[] = "ERR\r\n";

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
    
    //TODO: do we use DMA to capture RX bytes?
    
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
    
    INTCONbits.INT3EP = 0; //falling edge sensitive
    //INTEnable(INT_EXTERNAL_3_VECTOR, INT_ENABLED);
    
    currentResponseIndex = 0;
    responseComplete = 0;
    statusChanged = 0;
    status = 0;
    
    puts("S-,PrincessPhone\r"); //set friendly name
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    puts("SK,08\r"); //enable HFP profile
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    puts("SD,08\r"); //enable HFP discovery
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    return 0;
}

int rn52_isIdle(void)
{
    
    return 0;
}

int rn52_getStatus(void)
{
    puts("Q\r");
    gets(response);
    
    return (int)((response[1] << 8) | response[0]);
}

int rn52_getErr(void)
{
    return 0;
}

int rn52_dial(char* str)
{
    puts("A,"); //dial phone number
    puts(str);
    puts("\r");
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    return 0;
}

int rn52_accept(void)
{
    puts("C\r"); //accept call
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    return 0;
}

int rn52_reject(void)
{
    puts("E\r"); //end call
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    return 0;
}

int rn52_pairStart(void)
{
    puts("@,1\r"); //enable general discovery
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    return 0;
}

int rn52_pairStop(void)
{
    puts("@,0\r"); //disable discovery
    gets(response);
    if(strcmp(response, responseAOK)) while(1);
    
    return 0;
}

void __ISR(_EXTERNAL_3_VECTOR, ipl2) Int3Handler(void)
{
    //Stupid, I should have connected this to a pin with INT1 PPS capability.
    statusChanged = 1;
    
    IFS0bits.INT3IF = 0; //clear int flag
}

void __ISR(_UART_1_VECTOR, ipl2) Uart1Handler(void)
{
    //TODO: GRAB BYTES
    //QUES: USE DMA with pattern match INSTEAD?
    char c;
    c = U1RXREG;
    response[currentResponseIndex++] = c;
    
    if(c == '\r') responseComplete = 1;
    
    // clear the interrupt flag
    IFS1bits.U1RXIF = 0;
}

int rn52_tasks(void)
{
    //honestly we should probably just poll the change pin to save INT overhead...
    
    if(statusChanged) //check for status changes
    {
        status = rn52_getStatus();
        
        //accommodate changes as necessary
        switch((status & 0x0F00) >> 8) //connection state
        {
            case 3: //connected to an audio gateway
                break;

            case 6: //active call
                break;

            default:
                break;
        }
        
        statusChanged = 0;
    }
    
    return 0;
}

int rn52_isIncoming(void)
{
    return ((status & 0x0F00) == 0x0500);
}

int rn52_isOutgoing(void)
{
    return ((status & 0x0F00) == 0x0400);
}

int rn52_isActive(void)
{
    return ((status & 0x0F00) == 0x0600);
}