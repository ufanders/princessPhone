#include "HardwareProfile.h"
#include <plib.h>

void princessPhone_tasks(void)
{    
    //always do this stuff
    //TODO: check buttons
    //TODO: update indicator lamps
    
    if(AG1170_SHK_PORT)  //phone is off-hook, do outgoing telephone stuff.
    {
        if(rn52_isActive() || rn52_isOutgoing())
        {
            
        }
        else
        {
            if(!dtmf_dialtoneStatusGet())
            {
                dtmf_dialtoneStart(0); //start dialtone
            }

            rotary_stateMachine();

            if(rotary_numberIsReady()) //if a number has been tickled in, dial it.
            {
                dtmf_dialtoneStop(); //stop dialtone
                rn52_dial(rotary_getNumber()); //dial number
            }
        }
        
        //always when off-hook
        //TODO: adjust volume
    }
    else //phone is on-hook, do incoming telephone stuff and other stuff.
    {
        if(rn52_isIncoming()) //ring if call is incoming
        {
            ag1170_bellRingStart();
        }
        else
        {
            dtmf_dialtoneStop(); //stop dialtone if we hung up without dialing a number
            
            //TODO: if button 1 is down for 3 seconds, enter pairing mode.
        }
    }
}
