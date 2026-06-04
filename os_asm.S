.syntax unified       @Tells the assembler to use modern and standarzed ARM unified language
.cpu cortex-m0plus    @Tells the assembler to use ARMv6-M instruction set
.thumb                @Instructions to generate 16-bit Thumb Instruction

.global OS_Yield
.thumb_func

OS_Yield:
    LDR R0,=0xE000ED04 @Load the address of ICSR register
    LDR R1,=0x10000000 @Load the PENDSVSET bit mask constant
    STR R1, [R0]        @write the bitmask into the ICSR register
    DSB                @Data synchronization Barrier
    ISB                @Instuction synchronization barrier
    BX LR               @Return to C code

.global isr_pendsv
.type isr_pendsv, %function
.thumb_func
isr_pendsv:            @ Point the SDK vector label directly into our handler code
    .extern current_tcb @Address of Current TCB (from Task.h)
    .extern next_tcb    @Address of next TCB (from Task.h)
    
    //Saving Current TASKS Context
    MRS R0,PSP          @Read the current stack pointer value into working register
    
    // Decrement by 16 bytes to safely store high registers first
    SUBS R0,#16          
    MOV R4,R8           @READ R8 into R4
    MOV R5,R9           @REAN R5 into R9
    MOV R6,R10          @READ R6 into R10
    MOV R7,R11          @READ R7 into R11
    STMIA R0!, {R4-R7}  @PUSH R4-R7(R8-R11) on to the stack
    
    // Rewind back past the high registers and another 16 bytes for low registers
    SUBS R0,#32          
    STMIA R0!, {R4-R7}  @PUSH R4-R7 on to the stack
    
    // Final point correction to align with the bottom of the software stack (R4)
    SUBS R0,#16        
    
    LDR R1,=current_tcb @R1 is the address of current_tcb pointer variable
    LDR R1,[R1]         @The actual address of current TCB struct
    STR R0,[R1]         @Stack pointer back into current tasks TCB structure
    
    //updating TCB pointers (Scheduling logic)
    LDR R1,=current_tcb @loads physical mem address of current_tcb into R1 
    LDR R2,[R1]         @Reads the value inside current_tcb
    LDR R3,=next_tcb    @loads physical mem address of next_tcb into R3
    LDR R4,[R3]         @Reads the next_tcb variable value into R4
    STR R4,[R1]         @Writing TaskB's address into current_tcb (Task b is now officially active task)
    STR R2,[R3]         @writes TaskA tcb address into R3 which is next_tcb
    
    //Restore next tasks context
    LDR R0,[R4]         @R0 is the current_tcb pointer variable
    LDMIA R0!,{R4-R7}   @POP registers R4-R7
    
    LDMIA R0!,{R4-R7}   @POP registers R4-R7 (R8-R11)
    MOV R8,R4
    MOV R9,R5
    MOV R10,R6
    MOV R11,R7
    
    MSR PSP,R0    @Write this back into PSP
    BX LR