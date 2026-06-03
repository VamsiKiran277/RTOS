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
    
    //FORCE NVIC TO ENABLE EXCEPTION 14 (PendSV)
    volatile uint32_t *nvic_iser = (volatile uint32_t *)(0xE000E100);
    *nvic_iser |= (1U << 14);
    
}
void TaskA(void) {
    while(1) {
        //toggle
        *SIO_GPIO_OUT_XOR = PIN_OUT_SET;
       // for(volatile int i = 0; i < 500000; i++); //temporary delay until assembly switch works
       TASKA_COUNTER++;
       //switch hand control to TASK B
       OS_Yield();
    }
}

void TaskB(void) {
    while(1) {
        TASKB_COUNTER++;
        //just counting for now
        //switch control to TASK A
        OS_Yield();
    }
}

int main() {
    Hardware_init(); //initializing the Hardware

    //Passing the function name and global array and then capture topofstack ptr
    TCB_LIST[0].topStackPtr = TCB_Initialization(TaskA,TASKA_STACK);
    TCB_LIST[1].topStackPtr = TCB_Initialization(TaskB,TASKB_STACK);
    
//    irq_set_exclusive_handler(14, PendSV_Handler);
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

    __asm volatile("CPSIE i" : : : "memory"); // Clears PRIMASK globally
    //contect switch
    OS_Yield();

    while(1) {
        //unreachable once RTOS is live
    }
}