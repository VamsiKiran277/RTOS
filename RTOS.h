#ifndef RTOS_H
#define RTOS_H

//Macros definitons
#define RESET_BASE_ADDRESS  0x4000c000
#define IO_BANK0_BASE   0x40014000
#define SIO_BASE        0xd0000000
//OFFSETS
#define RESET_OFFSET  0x00
#define RESET_RESET_DONE   0x08
#define PIN25_CNTL   0xcc     //onboard LED
#define GPIO_OE_SET  0x24
#define GPIO_OUT_XOR 0x1c
//definitions
#define RESET_RESET   ((volatile unsigned int*)(RESET_BASE_ADDRESS + RESET_OFFSET))
#define RESET_DONE    ((volatile unsigned int*)(RESET_BASE_ADDRESS + RESET_RESET_DONE))
#define GPIO25_CNTL   ((volatile unsigned int*)(IO_BANK0_BASE + PIN25_CNTL))
#define SIO_GPIO_OUTPUT_SET ((volatile unsigned int*)(SIO_BASE + GPIO_OE_SET))
#define SIO_GPIO_OUT_XOR ((volatile unsigned int*)(SIO_BASE + GPIO_OUT_XOR))
//Bit funs
#define CLEAR_RESET   (1U<<5)
#define GPIO25_FUNCSEL 5
#define PIN_OUT_SET  (1U<<25)

//Function Declarations
void Hardware_init();
void TaskA(void);

#endif