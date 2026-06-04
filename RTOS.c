#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "RTOS.h"
#include "Task.h"

volatile uint32_t TASKA_COUNTER = 0;
volatile uint32_t TASKB_COUNTER = 0;

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
    //testing below for taskA  force PENDSV to the lowest
    volatile uint8_t *shpr3 = (volatile uint8_t *)(0xE000ED22);
    *shpr3 = 0xC0; // Set to lowest priority level
    
}
//Reload value = (Clock Cycle*desired_value)-1 //clock cycle is 16MHZ and desired value is 1ms(0.001)

void SysTick(void) {
    SysTick_config->SYST_RVR = ((Frequency/1000)-1);
    SysTick_config->SYST_CVR = 0;
    SysTick_config->SYST_CSR = 0x07; //enable,Interrupt and processor clock
}
void isr_systick(void) {
    *PENDSVSET_ICSR = PENDSVSET;
}

void TaskA(void) {
    while(1) {
        //toggle
        *SIO_GPIO_OUT_XOR = PIN_OUT_SET;  //automatically does the xor
       // for(volatile int i = 0; i < 500000; i++); //temporary delay until assembly switch works
       TASKA_COUNTER++;
       //switch control to TASK B
//       OS_Yield(); //Removing For now to test SysTick Handler
    }
}

void TaskB(void) {
    while(1) {
        TASKB_COUNTER++;
        //just counting for now
        //switch control to TASK A
//        OS_Yield();  //removing for now to test SysTick Handler
    }
}

int main() {
    Hardware_init(); //initializing the Hardware


    //Passing the function name and global array and then capture topofstack ptr
    TCB_LIST[0].topStackPtr = TCB_Initialization(TaskA,TASKA_STACK);
    TCB_LIST[1].topStackPtr = TCB_Initialization(TaskB,TASKB_STACK);
    

    //point HARDWARE PSP register to initialzed taskA  stack pointer
    __asm volatile("MSR psp, %0" : : "r" (TCB_LIST[0].topStackPtr) : "r0");
    //update the cpu control register to shift from MSP to PSP tracking
    __asm volatile(
        "MOVS r0, #2\n"
        "MSR control, r0\n"
        : : : "r0"
    );
    //Force Synchronization
    __asm volatile("ISB" : : : "memory");

    SysTick();

    __asm volatile("CPSIE i" : : : "memory"); // Clears PRIMASK globally
    //context switch
   // OS_Yield();
   TaskA(); //testing

    while(1) {
        //unreachable once RTOS is live
    }
}