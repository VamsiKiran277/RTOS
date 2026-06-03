#include "Task.h"

TCB_t TCB_LIST[TOTAL_TASKS];
uint32_t TASKA_STACK[STACK_SIZE];
uint32_t TASKB_STACK[STACK_SIZE];

TCB_t* current_tcb = &TCB_LIST[0];
TCB_t* next_tcb = &TCB_LIST[1];

uint32_t* TCB_Initialization(void(*task)(void),uint32_t*stack_arr) {
    uint32_t* topOfStack;
    topOfStack = (stack_arr + (STACK_SIZE)); //to get the top address of the stack
    
    //  Hardware Stack Frame (Popped automatically by CPU on exception return)
    topOfStack[-1] = 0x01000000; //standard as per ARM rules (xPSR with Thumb bit set)
    topOfStack[-2] = (uint32_t)task; //address of next task function (PC)
    topOfStack[-3] = 0xFFFFFFFD; // Link register (LR)
    topOfStack[-4] = 0x22222222; //R12
    topOfStack[-5] = 0x33333333; //R3
    topOfStack[-6] = 0x44444444; //R2
    topOfStack[-7] = 0x55555555; //R1
    topOfStack[-8] = 0xABCDEF01; //R0

    //Software Stack Frame (Popped manually by PendSV_Handler)
    topOfStack[-9]  = 0x11111111; // R11 
    topOfStack[-10] = 0x10101010; // R10 
    topOfStack[-11] = 0x09090909; // R9
    topOfStack[-12] = 0x08080808; // R8 
    topOfStack[-13] = 0x07070707; // R7 
    topOfStack[-14] = 0x06060606; // R6 
    topOfStack[-15] = 0x05050505; // R5 
    topOfStack[-16] = 0x04040404; // R4

    // Return the absolute lowest address of our complete 16-register context block
    return &topOfStack[-16];
}