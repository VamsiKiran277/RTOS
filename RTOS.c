#include <stdio.h>
#include "pico/stdlib.h"
#include "RTOS.h"
#include "Task.h"

void Hardware_init() {
    //CLEAR RESET bit for IO_BANK0
    *RESET_RESET &=~(CLEAR_RESET); 
    //wait for hardware to stabilize
    while(((*RESET_DONE) &(CLEAR_RESET)) == 0) {
        //DO nothing
    }
    //Function select for pin 25
    *GPIO25_CNTL = GPIO25_FUNCSEL;
    //OUTPUT set for pin 25
    *SIO_GPIO_OUTPUT_SET = PIN_OUT_SET;
    
}
void TaskA(void) {
    while(1) {
        //toggle
        *SIO_GPIO_OUT_XOR = PIN_OUT_SET;
        for(volatile int i = 0; i < 500000; i++); //temporary delay until assembly switch works
    }
}
int main() {
    Hardware_init(); //initializing the Hardware
    //Passing the function name and global array and then capture topofstack ptr
    TCB_LIST[0].topStackPtr = TCB_Initialization(TaskA,TASKA_STACK);
    //TCB_LIST[1].topStackPtr = TCB_Initialization(TaskB,TASKB_STACK); //YET TO IMPLEMENT TASK B

    //right now calling taskA to check register math later it will be replace by TCB initialization
//    TaskA();  Turned it off for now to check TCB
    //absolute_time_t start_time = get_absolute_time(); //get start time
    //absolute_time_t target_time = delayed_by_ms(start_time,1000);
    while(1) {
        //unreachable once RTOS is live
    }

}
