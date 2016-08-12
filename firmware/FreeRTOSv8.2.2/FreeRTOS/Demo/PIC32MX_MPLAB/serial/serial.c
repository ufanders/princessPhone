/* BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER. 

NOTE:  This driver is primarily to test the scheduler functionality.  It does
not effectively use the buffers or DMA and is therefore not intended to be
an example of an efficient driver. */

/* Standard include file. */
#include <stdlib.h>
#include <plib.h>
#include "HardwareProfile.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Demo app include files. */
#include "serial.h"

/* Hardware setup. */
#define serSET_FLAG						( 1 )

/* The queues used to communicate between tasks and ISR's. */
static QueueHandle_t xRxedChars; 
static QueueHandle_t xCharsForTx; 

/* Flag used to indicate the tx status. */
static volatile portBASE_TYPE xTxHasEnded;

/*-----------------------------------------------------------*/

/* The UART interrupt handler.  As this uses the FreeRTOS assembly interrupt
entry point the IPL setting in the following prototype has no effect.  The
interrupt priority is set by the call to  ConfigIntUART2() in 
xSerialPortInitMinimal(). */
void __attribute__( (interrupt(IPL0AUTO), vector(_UART1_VECTOR))) vU1InterruptWrapper( void );

/*-----------------------------------------------------------*/

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
        unsigned short usBRG;
  
	//Create the queues used by the com test task.
	xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
        if(!xRxedChars) while(1);
	xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
        if(!xCharsForTx) while(1);

	// Configure the UART and interrupts
        TRISAbits.TRISA1 = 0;
        TRISCbits.TRISC0 = 0;

        unsigned int status1, status2;
	mSYSTEMUnlock(status1, status2);
    
        U1RXR = 0b0110; //U1RX assigned to RPC1
        U1CTSR = 0b0000; // U1CTS assigned to RPA1
        RPC0R = 0b0001; // U1TX assigned to RPC0
        RPB10R = 0b0001; //U1RTS assigned to RPB10
        
        mSYSTEMLock(status1, status2);
     
        //UARTConfigure(UART1, UART_ENABLE_PINS_CTS_RTS);
        UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
        UARTSetDataRate(UART1, GetPeripheralClock(), ulWantedBaud);
        UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
        
        //UARTEnable(UART1, UART_ENABLE | UART_PERIPHERAL | UART_RX | UART_TX);
        
        U1STAbits.URXEN = 1;
        U1STAbits.UTXEN = 1;
        
        U1MODEbits.UEN = 0b00;
        U1MODEbits.BRGH = 0;
        U1MODEbits.PDSEL = 0b00;
        U1MODEbits.STSEL = 0;
        //U1MODEbits.ON = 1;
        
        INTSetVectorPriority(INT_UART_1_VECTOR, INT_PRIORITY_LEVEL_2);
        INTEnable(INT_U1RX, INT_ENABLED);
        INTEnable(INT_U1TX, INT_ENABLED);
        
        //U1TXREG = 0x55; //works fine!

	xTxHasEnded = pdTRUE;
    
        /*
        xSerialPutChar(1, 0x01, portMAX_DELAY );//works fine!
        xSerialPutChar(1, 0x02, portMAX_DELAY );//works fine!
        xSerialPutChar(1, 0x03, portMAX_DELAY );//works fine!
         */
    
	// Only a single port is implemented so we don't need to return anything.
	return NULL;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* Only one port is supported. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
signed portBASE_TYPE xReturn;

	/* Only one port is supported. */
	( void ) pxPort;

	/* Return false if after the block time there is no room on the Tx queue. */
	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) != pdPASS )
	{
		xReturn = pdFAIL;
	}
	else
	{
		xReturn = pdPASS;
	}

	if( xReturn != pdFAIL )
	{
		/* A critical section should not be required as xTxHasEnded will not be
		written to by the ISR if it is already 0. */
		if(  xTxHasEnded == pdTRUE )
		{
			xTxHasEnded = pdFALSE;
			IFS1SET = _IFS1_U1TXIF_MASK;
		}
	}

	return pdPASS;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
}
/*-----------------------------------------------------------*/

void vU1InterruptHandler( void )
{
/* Declared static to minimise stack use. */
static char cChar;
static portBASE_TYPE xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;

	/* Are any Rx interrupts pending? */
	if( IFS1bits.U1RXIF == 1)
	{
		while( U1STAbits.URXDA )
		{
			/* Retrieve the received character and place it in the queue of
			received characters. */
			cChar = U1RXREG;
			xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
		}
		IFS1CLR = _IFS1_U1RXIF_MASK;
	}

	/* Are any Tx interrupts pending? */
	if( IFS1bits.U1TXIF == 1 )
	{
		while( ( U1STAbits.UTXBF ) == 0 )
		{
			if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
			{
				/* Send the next character queued for Tx. */
				U1TXREG = cChar;
			}
			else
			{
				/* Queue empty, nothing to send. */
				xTxHasEnded = pdTRUE;
				break;
			}
		}

		IFS1CLR = _IFS1_U1TXIF_MASK;
	}

	/* If sending or receiving necessitates a context switch, then switch now. */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
