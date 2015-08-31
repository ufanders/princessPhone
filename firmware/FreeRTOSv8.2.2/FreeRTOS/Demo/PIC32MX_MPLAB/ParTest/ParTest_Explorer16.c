/* Scheduler includes. */
#include "FreeRTOS.h"

/* Demo app includes. */
#include "partest.h"

#define ptOUTPUT 	0
#define ptALL_OFF	0

/*-----------------------------------------------------------
 * Simple parallel port IO routines.
 *-----------------------------------------------------------*/

void vParTestInitialise( void )
{
	/* All LEDs output. */	
	TRISA = ptOUTPUT;	
	PORTA = ptALL_OFF;

	/* Disable the JTAG. */
	DDPCONbits.JTAGEN = 0;
}
/*-----------------------------------------------------------*/

void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
unsigned portBASE_TYPE uxLEDBit;

	/* Which port A bit is being modified? */
	uxLEDBit = 1 << uxLED;

	if( xValue )
	{
		/* Turn the LED on.   Use of the PORTASET register removes the need
		to use a critical section. */
		PORTASET = uxLEDBit;
	}
	else
	{
		/* Turn the LED off.  Use of the PORTACLR register removes the need
		to use a critical section. */
		PORTACLR = uxLEDBit;
	}
}
/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
unsigned portBASE_TYPE uxLEDBit;
	
	uxLEDBit = 1 << uxLED;

	/* Use of the PORTAINV register removes the need to use a critical section. */
	PORTAINV = uxLEDBit;
}



