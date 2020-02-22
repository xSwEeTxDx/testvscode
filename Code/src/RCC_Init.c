#include "RCC_Init.h"
#include "Error_Handler.h"

void vRCC_Init(){//
    RCC->CFGR = 0x00000000;
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));

    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    //Регистр RCC->CFGR - отвечает за делители шин
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //Настройка системной шины HPRE(AHB = SYSCLK/1)
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; //Настройка системной шины PPRE1(APB1 = HCLK/2)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; //Настройка системной шины PPRE2(APB2 = HCLK/1)

    RCC->CFGR &= ~RCC_CFGR_PLLMUL;//Отчистить бит PLLMULL
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;//Отчистить бит PLLSRC
    RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;//Отчистить бит PLLXTPRE

    RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;//Источник тактирование HSE, включили делитель, Кварц = 8МГц
    RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_PREDIV_DIV1;//HSE = Кварц/1
    RCC->CFGR |= RCC_CFGR_PLLMUL9;//HSE = (Кварц/1)*9 = 72МГц

    RCC->CR |= RCC_CR_PLLON;//enable PLL
    while(!(RCC->CR & RCC_CR_PLLRDY)){}//Ожидаем готовности PLL

    RCC->CFGR &= ~RCC_CFGR_SW;//Очищаем биты SW(System clock switch)
    RCC->CFGR |= RCC_CFGR_SW_PLL;//Выбираем за основу PLL множитель
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1){}//Ожидаем запуска PLL
}