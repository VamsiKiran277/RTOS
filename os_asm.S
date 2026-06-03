.syntax unified       @Tells the assembler to use modern and standarzed ARM unified language
.cpu cortex-m0plus    @Tells the assembler to use ARMv6-M instruction set
.thumb                @Instructions to generate 16-bit Thumb Instruction

.global OS_Yield
.thumb_func

OS_Yield:
    LDR R0,=0xE000ED04 @Load the address of ICSR register
    LDR R1,=0x10000000 @Load the PENDSVSET bit mask constant
    STR R0,[R1]        @write the bitmask into the ICSR register
    DSB                @Data synchronization Barrier
    ISB                @Instuction synchronization barrier
    BX LR              @Return to C code