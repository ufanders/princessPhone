// These definitions are set in the MPLAB Project settings.  If you are starting
// a new project, you should start by modifying one of the pre-existing .mcp 
// files.  To modify the macro used, in MPLAB IDE, click on Project -> Build 
// Options... -> Project -> MPLAB XXX C Compiler -> Preprocessor Macros -> 
// Add.... Note that you may also have to add this macro to the assembler 
// (MPLAB XXX Assembler tab).

#define _SUPPRESS_PLIB_WARNING

#if defined(CFG_E16_460)
        #include "HWP_E16_460.h"
#elif defined(CFG_E16_270)
        #include "HWP_E16_270.h"
#elif defined(CFG_PP_A)
        #include "HWP_PP_A.h"
#else
	#error "No extended HWP .h included.  Add the appropriate compiler macro to the MPLAB project."
#endif

