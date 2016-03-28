#ifndef HWP_E16_270_H
#define HWP_E16_270_H

#define VERSION_HARDWARE_MAJOR 'E'
#define VERSION_HARDWARE_MINOR 16

// Set configuration fuses (but only in Main.c where SET_CONFIG_FUSES is defined)
#if defined(SET_CONFIG_FUSES)

    //PROVIDES 48MHZ FCY
    #pragma config FPLLMUL  = MUL_24        // PLL Multiplier
    #pragma config FPLLIDIV = DIV_2          // PLL Input Divider
    #pragma config FPLLODIV = DIV_2     // PLL Output Divider
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
#define GetSystemClock()	(48000000ul)		// Hz
#define GetInstructionClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.
#define SYS_FREQ GetSystemClock()
#define PB_DIV 1

// Hardware I/O pin mappings
//UART
#define UART_RX_TRIS TRISCbits.TRISC1 //RB10 on riser card
#define UART_TX_TRIS TRISCbits.TRISC0 //RB11 on riser card
#define UART_RTS_TRIS TRISBbits.TRISB10 //RGO on riser card
#define UART_CTS_TRIS TRISAbits.TRISA1 //RE6on riser card
#define UART_RX_PPS 0b0110
#define UART_TX_PPS 0b0001
#define UART_RTS_PPS 0b0010
#define UART_CTS_PPS 0b0000

//AG1170 CONTROL LINES
#define AG1170_RM_TRIS TRISCbits.TRISC3 //RG13 on riser card
#define AG1170_FR_TRIS TRISCbits.TRISC4 //RG14 on riser card
#define AG1170_SHK_TRIS TRISCbits.TRISC5 //RG15 on riser card
#define AG1170_PD_TRIS TRISCbits.TRISC6
#define AG1170_RM_LAT LATCbits.LATC3 //RG13 on riser card
#define AG1170_FR_LAT LATCbits.LATC4 //RG14 on riser card
#define AG1170_SHK_LAT LATCbits.LATC5 //RG15 on riser card
#define AG1170_PD_LAT LATCbits.LATC6
#define AG1170_RM_PORT PORTCbits.RC3 //RG13 on riser card
#define AG1170_FR_PORT PORTCbits.RC4 //RG14 on riser card
#define AG1170_SHK_PORT PORTCbits.RC5 //RG15 on riser card
#define AG1170_PD_PORT PORTCbits.RC6

//MCU AUDIO LINES
#define PWM_TRIS TRISBbits.TRISB4
#define PWM_PPS 0b0101


//RN52 CONTROL LINES
#define RN52_DFU_MODE_TRIS TRISBbits.TRISB9
#define RN52_FACTORY_RST_TRIS TRISBbits.TRISB11
#define RN52_PWREN_TRIS TRISCbits.TRISC7
#define RN52_EVENT_L_TRIS TRISCbits.TRISC8
#define RN52_CMD_MODE_L_TRIS TRISCbits.TRISC9
#define RN52_DFU_MODE_LAT LATBbits.LATB9
#define RN52_FACTORY_RST_LAT LATBbits.LATB11
#define RN52_PWREN_LAT LATCbits.LATC7
#define RN52_EVENT_L_LAT LATCbits.LATC8
#define RN52_CMD_MODE_L_LAT LATCbits.LATC9

//LEDS
#define LED_RED_TRIS TRISAbits.TRISA10
#define LED_GRN_TRIS TRISAbits.TRISA7
#define LED_RGB_RED_TRIS TRISBbits.TRISB14
#define LED_RGB_GRN_TRIS TRISAbits.TRISA0
#define LED_RGB_BLU_TRIS TRISAbits.TRISA2
#define LED_RED_LAT LATAbits.LATA10
#define LED_GRN_LAT LATAbits.LATA7
#define LED_RGB_RED_LAT LATBbits.LATB14
#define LED_RGB_GRN_LAT LATAbits.LATA0
#define LED_RGB_BLU_LAT LATAbits.LATA2

//BUTTONS
#define SW0_TRIS TRISBbits.TRISB5 //RG13 on riser card
#define SW0_PORT PORTBbits.RB5
#define SW0_PU CNPUBbits.CNPUB5
#define SW1_TRIS TRISBbits.TRISB6 //RG13 on riser card
#define SW1_PORT PORTBbits.RB6
#define SW1_PU CNPUBbits.CNPUB6
#define SW2_TRIS TRISBbits.TRISB7 //RG13 on riser card
#define SW2_PORT PORTBbits.RB7
#define SW2_PU CNPUBbits.CNPUB7
        
//SPI
#define SPI_SCK_TRIS TRISBbits.TRISB15 //RC2 on riser card
#define SPI_MOSI_TRIS TRISBbits.TRISB13 //RG14 on riser card
#define SPI_MISO_TRIS TRISCbits.TRISC2 //RG14 on riser card
#define SPI_CS_TRIS TRISBbits.TRISB12 //RE5 on riser card
#define SPI_SCK_LAT LATBbits.LATB15
#define SPI_MOSI_LAT LATBbits.LATB13
#define SPI_MISO_LAT LATCbits.LATC2 //RG14 on riser card
#define SPI_CS_LAT LATBbits.LATB12
#define SPI_SCK_PPS RPB15R
#define SPI_MOSI_PPS RPB15R
#define SPI_MISO_PPS RPB15R

//I2C
#define I2C2_SDA_TRIS TRISBbits.TRISB2 //RC2 on riser card
#define I2C2_SCL_TRIS TRISBbits.TRISB3 //RG14 on riser card

#endif	/* HWP_E16_270_H */

