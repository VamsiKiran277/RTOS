#include "Task.h"

TCB_t TCB_LIST[TOTAL_TASKS];
uint32_t TASKA_STACK[STACK_SIZE];
uint32_t TASKB_STACK[STACK_SIZE];

uint32_t* TCB_Initialization(void(*task)(void),uint32_t*stack_arr) {
    uint32_t* topOfStack;
    topOfStack = (stack_arr + (STACK_SIZE-1)); //to get the top address of the stack
    return topOfStack;
}  