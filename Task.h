#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "RTOS.h"

#define TOTAL_TASKS  2  //Change it later if you want more tasks
#define STACK_SIZE 128


typedef struct {
    uint32_t *topStackPtr;
    //later we can add more items for priority scheduing
}TCB_t;

extern TCB_t TCB_LIST[TOTAL_TASKS];
//allocate independent taks stack size
extern uint32_t TASKA_STACK[STACK_SIZE];
extern uint32_t TASKB_STACK[STACK_SIZE];

//sp that assembly can access them using .extern
extern TCB_t* current_tcb;
extern TCB_t* next_tcb;



//initialization function with accepting address from a task and a pointer to the base of the stack array
uint32_t* TCB_Initialization(void(*task)(void),uint32_t*stack_arr);  