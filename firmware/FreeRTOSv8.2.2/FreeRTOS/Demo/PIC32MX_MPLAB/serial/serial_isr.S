#include <p32xxxx.h>
#include <sys/asm.h>
#include "ISR_Support.h"

	.set	nomips16
	.set 	noreorder
 	
	.extern vU1InterruptHandler
	.extern xISRStackTop
	.global	vU1InterruptWrapper

	.set	noreorder
	.set 	noat
	.ent	vU1InterruptWrapper

vU1InterruptWrapper:

	portSAVE_CONTEXT
	jal vU1InterruptHandler
	nop
	portRESTORE_CONTEXT

	.end	vU1InterruptWrapper

