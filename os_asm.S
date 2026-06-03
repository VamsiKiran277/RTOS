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

.global PendSV_Handler
.global isr_pendsv         @ Export the SDK's expected vector name
.thumb_func

.global isr_pendsv      @ Expose the exact name the Pico SDK vector table is expecting
.type isr_pendsv, %function
.thumb_func

isr_pendsv:            @ The hardware will now jump straight here on PendSV
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
    LDR R1,=current_tcb
    LDR R2,[R1]
    LDR R3,=next_tcb
    LDR R4,[R3]
    STR R4,[R1]
    STR R2,[R3]
    
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