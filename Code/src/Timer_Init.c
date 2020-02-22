#include "Timer_Init.h"
//Сигнал RET или RESET более 50мкс
#define WS2812B_TIMER_RET       (89 * 45)
void vTIM3_Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    TIM3->PSC = 0;// Предделитель генерирует частоту 72 МГц
    TIM3->ARR = 3600-1;// Перегрузка таймера каждые 1.25 мкс
    TIM3->CCR3 = 0;
    TIM3->CNT = 0;

    TIM3->CCER = 0;
    TIM3->CCER |= TIM_CCER_CC3E;

    //Chanel3_TIM3 | 0110: PWM mode 1(upcounter)
    TIM3->CCMR2 =  TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
    
    //Включаем тактирование (канал 3 тим 3) внутри МК
    TIM3->DIER = TIM_DIER_CC3DE | TIM_DIER_UIE;

    TIM3->CR1 = TIM_CR1_CEN | TIM_CR1_ARPE;//1: TIMx_ARR register is buffered.
    
    NVIC_EnableIRQ(TIM3_IRQn);
}

void bus_retcode()
{
  TIM3->CR1 &= ~(TIM_CR1_CEN); //останавливаем таймер
  TIM3->ARR = WS2812B_TIMER_RET; //Устанавливаем период немного больше 50мкс
  TIM3->CNT = 0; //Очищаем счетный регистр
  TIM3->CCR3 = 0x0000; //значение ШИМ-а ноль
  TIM3->SR &= ~(TIM_SR_UIF); //сбрасываем флаг прерывания
  TIM3->DIER |= TIM_DIER_UIE; //прерывание по обновлению
  TIM3->CR1 |= TIM_CR1_CEN; //Поехали считать!
}

