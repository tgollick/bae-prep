# State Diagram

This project aims to create a finite state machine following an arm and disarm structure similar to what I am expected to face at BAE. 

This is to ensure I am implementing similar patterns and understand the chalanges faced with such memory/enviroment restrictions alongside learning more about safety protocalls, RTOS, schedulers etc. but many of these topics are covered later down the line.

States and Commands:
SAFE - This is the initial state and is when the mechinims are in place to stop the missle/torpedo from firing accidently due to a hardware or mechanical error.

ARMING - An ARM_CMD is provided by a user which starts a process of removing the mechanisms in which make the missle/torpedo safe. This is done one by one in a specific order rather than all at once to again add an additional layer of safety.

During this process if any of the safety mechanisms fail or for another reason the arming state fails, a FAULT_DETECTED command is automatically issued by internal system and the state transitions to FAULT.

When arming is complete and all safety mechanisms are removed the ARMING_COMPLETE internal command is issued transitioning the state to ARMED.

ARMED - 
ENABLED
FIRED
FAULT
DISARMING


