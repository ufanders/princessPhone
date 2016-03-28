#ifndef HWP_E16_460_H
#define	HWP_E16_460_H

#include "../Microchip/include/Compiler.h"

#define VERSION_HARDWARE_MAJOR 'E'
#define VERSION_HARDWARE_MINOR 16

// Set configuration fuses (but only in Main.c where SET_CONFIG_FUSES is defined)
#if defined(SET_CONFIG_FUSES)

    #pragma config FPLLMUL  = MUL_20        // PLL Multiplier
    #pragma config FPLLIDIV = DIV_2 //DIV_4         // PLL Input Divider
    #pragma config FPLLODIV = DIV_2 //DIV_1         // PLL Output Divider
    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    #pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config IESO     = OFF           // Internal/External Switch-over
    #pragma config FSOSCEN  = ON           // Secondary Oscillator Enable
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection

    #ifdef __DEBUG
        #pragma config CP       = OFF       // Code Protect
        #pragma config FWDTEN   = OFF       // Watchdog Timer
    #else
        #pragma config CP       = OFF        // Code Protect
        #pragma config FWDTEN   = OFF        // Watchdog Timer
    #endif

    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
    #pragma config DEBUG    = ON            // Background Debugger Enable
    #pragma config FUSBIDIO = OFF
    #pragma config FVBUSONIO = OFF

#endif

// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()	(40000000ul)		// Hz
#define GetInstructionClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.

//Firmware block inclusions
//Microchip Technology "MPFS2" filesystem
#define STACK_USE_MPFS2
#define MPFS_USE_SPI_FLASH
#define MAX_MPFS_HANDLES 1

//Framesync timer mapping
#define FRAMESYNC_TIMER_PERIOD PR1
#define FRAMESYNC_TIMER_CONBITS T1CONbits

// Hardware I/O pin mappings
//SPI
#define LED_SPI_SCK_TRIS TRISBbits.TRISB14 //RC2 on riser card
#define LED_SPI_MOSI_TRIS TRISBbits.TRISB5 //RG14 on riser card
#define LED_SPI_CS_TRIS TRISBbits.TRISB7 //RE5 on riser card
#define LED_SPI_SCK_LAT LATBbits.LATB14
#define LED_SPI_MOSI_LAT LATBbits.LATB5
#define LED_SPI_CS_LAT LATBbits.LATB7

#define MEM_SPI_SCK_TRIS TRISBbits.TRISB14 // on riser card
#define MEM_SPI_MOSI_TRIS TRISBbits.TRISB1 // on riser card
#define MEM_SPI_CS_TRIS TRISBbits.TRISB2 // on riser card
#define MEM_SPI_SCK_LAT LATBbits.LATB14
#define MEM_SPI_MOSI_LAT LATBbits.LATB1
#define MEM_SPI_CS_LAT LATBbits.LATB2

#define SPIFLASH_CS_TRIS	TRISBbits.TRISB11
#define SPIFLASH_CS_IO		LATBbits.LATB11
#define SPIFLASH_SCK_TRIS	TRISBbits.TRISB15
#define SPIFLASH_SDI_TRIS	TRISCbits.TRISC3
#define SPIFLASH_SDI_IO		LATCbits.LATC3
#define SPIFLASH_SDO_TRIS	TRISBbits.TRISB13
#define SPIFLASH_SPI_IF		IFS1bits.SPI2RXIF
#define SPIFLASH_SSPBUF		SPI2BUF
#define SPIFLASH_SPICON1	SPI2CON
#define SPIFLASH_SPICON1bits	SPI2CONbits
#define SPIFLASH_SPISTATbits	SPI2STATbits
#define SPIFLASH_SPIBRG		SPI2BRG

//UART
#define UART_RX_TRIS TRISCbits.TRISC1 //RB11 on riser card
#define UART_TX_TRIS TRISBbits.TRISB3 //RD4 on riser card

//CONTROL LINES
#define BT_RESET_TRIS TRISAbits.TRISA0 //RE7 on riser card
#define BT_RESET_LAT LATAbits.LATA0
#define BT_CONNECTED_TRIS TRISAbits.TRISA1 //RE6 on riser card
#define BT_CONNECTED_PORT PORTAbits.RA1

#define CPLD_RESET_TRIS TRISAbits.TRISA8 //RD1 on riser card
#define CPLD_RESET_LAT LATAbits.LATA8
#define CPLD_CLK_TRIS TRISCbits.TRISC0 //RB10 on riser card
#define CPLD_CLK_LAT LATCbits.LATC0

#define SLOW_CLK_TRIS TRISCbits.TRISC6 // on riser card
#define SLOW_CLK_ODC ODCCbits.ODCC6
#define SLOW_CLK_RP_REG RPC6R

//BUTTONS
#define SW_BUTTON_1_TRIS TRISCbits.TRISC9 //RG13 on riser card
#define SW_BUTTON_1_PORT PORTCbits.RC9
#define SW_BUTTON_1_PU CNPUCbits.CNPUC9

//EEPROM LINES
#define EEPROM_CS_TRIS	TRISCbits.TRISC2 //RG6 on riser card
#define EEPROM_CS_LAT	LATCbits.LATC2

#endif	/* HWP_EXPLORER16_H */

