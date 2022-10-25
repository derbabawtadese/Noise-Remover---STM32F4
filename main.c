/*
    1.  first we run the code wihout SystemClock_Config()
        and the clock was 16MHz; after adding SystemClock_Config()
        the system clock is 168MHz.
    2.  
    
    
*/

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "arm_math.h"                   // ARM::CMSIS:DSP
#include "filter_coeff.h"
#include "input_signal.h"

#define BLOCK_SIZE  32                  // Size of the block

//uint32_t freq;
//uint32_t count;

uint32_t numBlocks = SIGNAL_LENGTH1 / BLOCK_SIZE;                // Number ofblocks

float32_t inputSignal, outputSignal, outputSignal_5Hz;
static float32_t firState_f32[BLOCK_SIZE + NUM_TAPS - 1];         // Intermediate array for calculation
float32_t outputSignal_f32[SIGNAL_LENGTH1];

extern void SystemClock_Config(void);
void plotOutputSignal(void);
void plotInputSignal(void);
void plotBothSignal(void);
void plotOutput_5Hz_Signal(void);


int main()
{
    HAL_Init();
    SystemClock_Config();
    
    arm_fir_instance_f32 _1KHz_15KHz_sig;               // Create FIR filter instance 
    
    // init arguments: 
    // 1 - address of fir instance; 2 - address of fir coefficient array; 
    // 3 - address of intermmidiate array; 4 - block size
    // (float32_t *) - becuase it is defined as a constance which is in a diffrent memory location
    arm_fir_init_f32(&_1KHz_15KHz_sig, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firState_f32[0], BLOCK_SIZE);
    
    for(int i = 0; i < numBlocks; i++)
    {
        // arguments: 1 - address of fir instance; 2 - address of input signal array + i * BLOCK_SIZE to filter/store the signal block by block;
        // 3 - address of the output array to store the filtered signal; 4 - block size
        arm_fir_f32(&_1KHz_15KHz_sig, &inputSignal_f32_1kHz_15kHz[0] + (i * BLOCK_SIZE), &outputSignal_f32[0] + (i * BLOCK_SIZE), BLOCK_SIZE);
    }
    
//    freq = HAL_RCC_GetHCLKFreq();
    
    while (1)
    {
        //plotOutput_5Hz_Signal(); 
        plotBothSignal();
    }
}

void SysTick_Handler()
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void plotInputSignal(void)
{
    for(uint32_t i = 0; i < SIGNAL_LENGTH1; i++)
    {
        inputSignal = inputSignal_f32_1kHz_15kHz[i];
        if(i == SIGNAL_LENGTH1 - 1) i = 0;
        HAL_Delay(1);
    }
}

void plotOutputSignal(void)
{
    for(uint32_t i = 0; i < SIGNAL_LENGTH1; i++)
    {
        outputSignal = outputSignal_f32[i];
//        if(i == SIGNAL_LENGTH11 - 1) i = 0;
        HAL_Delay(1);
    }
}

void plotOutput_5Hz_Signal(void)
{
    for(uint32_t i = 0; i < SIGNAL_LENGTH2; i++)
    {
        outputSignal_5Hz = inputSignal_f32_5Hz[i];
        if(i == SIGNAL_LENGTH2 - 1) i = 0;
        HAL_Delay(1);
    }
}

void plotBothSignal(void)
{
    for(uint32_t i = 0; i < SIGNAL_LENGTH1; i++)
    {
        inputSignal = inputSignal_f32_1kHz_15kHz[i];
        outputSignal = outputSignal_f32[i];
        if(i == SIGNAL_LENGTH1 - 1) i = 0;
        HAL_Delay(1);
    }
}
