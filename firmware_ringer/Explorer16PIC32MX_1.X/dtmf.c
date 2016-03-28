//TODO: dial tone

#include "HardwareProfile.h"
#include <plib.h>

/* 
 * dial tone is a continuous tone having frequencies of 350 and 440 Hz at a level of −13 dBm
*/

// Global Variables
unsigned int waveA; //   absolute 0x15;   // 32bit accumulators for the 2 sinewaves
//unsigned char waveA_2 absolute 0x17;   // overload for fast access to byte 2
unsigned int waveB; //  absolute 0x19;  
//unsigned char waveB_2 absolute 0x1B;   // overload for fast access to byte 2

unsigned int BDA_periodA;  // 32bit periods for the 2 sinewaves
unsigned int BDA_periodB;

// sinewave table, range 0-99, 64 entries
const unsigned char sine64[64] = {
    50,54,59,64,68,73,77,81,
    85,88,91,93,95,97,98,99,
    99,99,98,97,95,93,91,88,
    85,81,77,73,68,64,59,54,
    50,45,40,35,31,26,22,18,
    14,11,8,6,4,2,1,0,
    0,0,1,2,4,6,8,11,
    14,18,22,26,31,35,40,45
};

/*
// defines for the DTMF sine frequencies
// Sinewave is a lookup table of 64 steps, to generate 1633 Hz each step is 1633 * 64
// 24bit Binary divider uses byte2 (/65536)
// PWM freq is 128 PIC instructions (8Mhz xtal *PLL) = 62500 Hz
// So the math is;  Fout * 64 * 65536 / Fpwm
// we can use a constant Fout * (64 * 65536 / 62500) = Fout * 67.108864
#define Frow0 46775   // 697 Hz  DTMF rows
#define Frow1 51674   // 770 Hz
#define Frow2 57177   // 852 Hz
#define Frow3 63149   // 941 Hz

#define Fcol0 81135   // 1209 Hz  DTMF columns
#define Fcol1 89657   // 1336 Hz
#define Fcol2 99120   // 1477 Hz
#define Fcol3 109589  // 1633 Hz
*/

/*
 * FPWM = 48000000 / 8 / 128 = 46875
 * DIALTONE_350 = (350*64*65536)/46875 = 31317
 */
#define DIALTONE_350 31317
#define DIALTONE_440 39371
#define BUSYTONE_480 42950
#define BUSYTONE_620 55477

int dtmf_init(void)
{
    //DTMF tone generation really ought to be a DMA affair...
    
    //configure OC1 timebase (Timer2)
    //8-tick prescale further divided into a 128-cycle PWM clock = (8/128) = 46875
    OpenTimer2(T2_OFF | T2_SOURCE_INT | T2_PS_1_8, (GetPeripheralClock()/46875));
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
    
    //configure OC1 (PWM module)
    OpenOC1( 0, 0, \
            (OC_OFF | OC_IDLE_CON | OC_TIMER_MODE16 | OC_TIMER2_SRC | OC_HIGH_LOW));
    //configure OC1 interrupt
    //ConfigIntOC1((OC_INT_PRIOR_2 | OC_INT_SUB_PRI_0 | OC_INT_OFF));
    
    //configure OC1 PPS
    int intStat, dmaSusp;
    mSYSTEMUnlock(intStat, dmaSusp);
    RPB4R = PWM_PPS;
    mSYSTEMLock(intStat, dmaSusp);
    
    PWM_TRIS = 0;
    
    return 0;
}

int dtmf_dialtoneStart(int region)
{
    BDA_periodA = DIALTONE_350;
    BDA_periodB = DIALTONE_440;
    
    OC1CONbits.ON = 1;
    T2CONbits.ON = 1;
    
    return 0;
}

int dtmf_dialtoneStop(void)
{
    OC1CONbits.ON = 0;
    T2CONbits.ON = 0;
}

int dtmf_dialtoneStatusGet(void)
{
    return OC1CONbits.ON;
}

int dtmf_busytoneStart(int region)
{
    BDA_periodA = BUSYTONE_480;
    BDA_periodB = BUSYTONE_620;
    
    OC1CONbits.ON = 1;
    T2CONbits.ON = 1;
    
    return 0;
}

int dtmf_busytoneStop(void)
{
    OC1CONbits.ON = 0;
    T2CONbits.ON = 0;
}

int dtmf_busytoneStatusGet(void)
{
    return OC1CONbits.ON;
}

void __ISR(_TIMER_2_VECTOR, ipl2) Timer2Handler(void)
{
    //-------------------------------------------------------
    // this generates the 2 accurate sinewaves together.
    // a DDS algorithm calculation is done once every PWM cycle,
    // then the 2 sinewaves are generated by the PIC PWM module.
    // For a full description of algorithm; www.RomanBlack.com/one_sec.htm#BDA
    // NOTE! for good speed PWM TMR2 is manually polled, not an interrupt.
    //-------------------------------------------------------
    unsigned char pwm;

    // calc the A sinewave, 
    waveA += BDA_periodA;	        // zero-error Accumulation 
    pwm = sine64[(waveA/65536) & 0x3F]; // Binary Divide output (/65536) and keep 6 bits 

    // calc the B sinewave, and ADD the 2 waves together
    waveB += BDA_periodB;	              
    pwm += sine64[(waveB/65536) & 0x3F]; // Binary Divide output (/65536) and keep 6 bits 

    // scale the summed sinewaves and place in PWM module
    pwm = (pwm >> 1);   // scale 0-200 back to 0-100 for PWM
    pwm += 14;          // offset to centre waveform (100 into centre of 128)
    OC1R = pwm;       // load added sinewaves into PWM module
    
    IFS0bits.T2IF = 0; //clear interrupt flag
}