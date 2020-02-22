#include "stm32f3xx.h"
#include "Timer_Init.h"

void vClear_DMA();
void vDMA1_Channel2_Mem_To_TIM3_Init();
//void DMA1_Channel2_IRQHandler();
void ws2812b_buff_claer();
int ws2812b_set(int pixn, uint8_t r, uint8_t g, uint8_t b);
void ws2812b_send(void);