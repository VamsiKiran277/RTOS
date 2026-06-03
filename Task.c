#include "Task.h"

TCB_t TCB_LIST[TOTAL_TASKS];
uint32_t TASKA_STACK[STACK_SIZE];
uint32_t TASKB_STACK[STACK_SIZE];

uint32_t* TCB_Initialization(void(*task)(void),uint32_t*stack_arr) {
    uint32_t* topOfStack;
    topOfStack = (stack_arr + (STACK_SIZE)); //to get the top address of the stack
    //fake addresses
    topOfStack[-1] = 0x01000000; //standard as per ARM rules
    topOfStack[-2] = (uint32_t)task; //address of next task function
    topOfStack[-3] = 0x11111111; // Link register
    topOfStack[-4] = 0x22222222; //R12
    topOfStack[-5] = 0x33333333; //R3
    topOfStack[-6] = 0x44444444; //R2
    topOfStack[-7] = 0x55555555; //R1
    topOfStack[-8] = 0xABCDEF01; //R0


    return &topOfStack[-8];
}  