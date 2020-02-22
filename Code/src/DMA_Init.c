#include "DMA_Init.h"

#define WS2812B_TIMER_AAR 89
#define WS2812B_NUM_LEDS (5)
#define DATA_LEN ((WS2812B_NUM_LEDS * 24))
#define WS2812B_0_VAL (WS2812B_TIMER_AAR / 3)
int flag_stop = 0;
//Передача лог. единицы 0.85мкс
#define WS2812B_1_VAL           (61)

static uint16_t led_array[DATA_LEN];

static int flag_rdy = 0;

void vClear_DMA()
{
  DMA1->IFCR = 0x0FFFFFFF;  // Очитить флаги во всех каналах DMA
}

void vDMA1_Channel2_Mem_To_TIM3_Init(){
  for(int i = 0; i < DATA_LEN; i++){
      led_array[i] = WS2812B_1_VAL;
  }
  // разрешаем тактирование ДМА1
  if((RCC->AHBENR & RCC_AHBENR_DMA1EN) != RCC_AHBENR_DMA1EN)
  {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  }

  DMA1_Channel2->CCR = 0;
  DMA1_Channel2->CPAR = (uint32_t)&TIM3->CCR3;
  DMA1_Channel2->CMAR = (uint32_t)&led_array;
  DMA1_Channel2->CNDTR = DATA_LEN;

  DMA1_Channel2->CCR = 
                      DMA_CCR_MINC | DMA_CCR_CIRC 
                    | DMA_CCR_DIR | DMA_CCR_MSIZE_0 
                    | DMA_CCR_PSIZE_0 | DMA_CCR_PL | DMA_CCR_TCIE;

  NVIC_EnableIRQ(DMA1_Channel2_IRQn);
}

void DMA1_Channel2_IRQHandler(){
  //RCC->AHBENR &= ~RCC_AHBENR_DMA1EN;
  DMA1->IFCR = DMA_IFCR_CTCIF2;
  
  flag_stop = 1;
}

void ws2812b_send(void)
{
  if(flag_rdy) //Если сейчас ни чего не передается
  {
    //Устанавливаем флаг занятости интерфейса
    flag_rdy = 0;
    
    //Настраиваем передачу данных
    DMA1_Channel2->CCR &= ~DMA_CCR_EN; //Отключаем канал DMA
    DMA1_Channel2->CNDTR = sizeof(led_array); //Устанавливаем количество данных
    
    //Таймер считает до WS2812B_TIMER_AAR, таким образом
    //при данной частоте тактирования таймера
    //получаем период ШИМ-сигнала, равный 1.25мкс
    TIM3->ARR = WS2812B_TIMER_AAR;
    TIM3->CCR3 = 0x0000; //Устанавливаем ШИМ-регистр таймера в ноль
    TIM3->CNT = 0; //Очищаем счетный регистр
    TIM3->CR1 |= TIM_CR1_CEN; //Запускаем таймер
    //Так как значение ШИМ установили в ноль, 
    //то на шине будет установлен неактивный уровень
    //до момента запуска DMA  
    
    DMA1->IFCR |= DMA_IFCR_CTEIF2 | DMA_IFCR_CHTIF2 
      | DMA_IFCR_CTCIF2 | DMA_IFCR_CGIF2; //Очищаем все флаги прерываний DMA
    
    DMA1_Channel2->CCR |= DMA_CCR_TCIE; //прерывание завершения передачи
    
    //Включаем канал DMA, тем самым начинаем передачу данных
    DMA1_Channel2->CCR |= DMA_CCR_EN; 
  }
}
int tdelay = 0;//1.25мкс
void TIM3_IRQHandler(void)
{
  if(tdelay == 20){tdelay = 0; DMA1_Channel2->CCR |= DMA_CCR_EN;}
  
  if(TIM3->SR & TIM_SR_UIF) // if UIF flag is set
  { 
    if(flag_stop)
    {
      TIM3->CR1 &= ~TIM_CR1_CEN; //останавливаем таймер
      TIM3->DIER &= ~TIM_DIER_UIE; //запрещаем прерывание таймера
    }
    TIM3->SR &= ~TIM_SR_UIF; // clear UIF flag
    
  }tdelay++;//52.75us
}