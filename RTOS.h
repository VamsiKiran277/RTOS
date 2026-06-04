#ifndef RTOS_H
#define RTOS_H

//Macros definitons
#define RESET_BASE_ADDRESS  0x4000c000
#define IO_BANK0_BASE   0x40014000
#define SIO_BASE        0xd0000000
#define SYSTICK_BASE    0xE000E010
#define ICSR_BASE       0xE000ED04
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
#define SysTick_config        ((SysTick_t*)SYSTICK_BASE)
#define PENDSVSET_ICSR ((volatile unsigned int*)(ICSR_BASE))

//Bit funs
#define CLEAR_RESET   (1U<<5)
#define GPIO25_FUNCSEL 5
#define PIN_OUT_SET  (1U<<25)
#define PENDSVSET    (1U<<28)
//Clock Frequency
#define Frequency    125000000

typedef struct {
    uint32_t SYST_CSR;//control and status register //offset at 0x00 
    uint32_t SYST_RVR;//Reload Value Register //offset at 0x04
    uint32_t SYST_CVR;//Current Value Register //offset at 0x08
    uint32_t SYST_CALIB;//Calibration value register //offset at 0xc 
}SysTick_t;

//Function Declarations
void Hardware_init();
void TaskA(void);
void TaskB(void);
extern void OS_Yield(void);
extern void PendSV_Handler(void);
void SysTick(void);
void isr_systick(void);

#endif