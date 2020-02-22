#include "GPIO_Init.h"

void vGPIO_Init(){
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
}

void vGPIOA(){
    //*****PA5 - output_mode|output_push-pull|High_speed|No_pull-up_pull-down*****
    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;//[1:0]00: Input mode (reset state)|01: General purpose output mode|10: Alternate function mode|11: Analog mode
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;//[1:0]0: Output push-pull (reset state)|1: Output open-drain
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;//[1:0]x0: Low speed|01: Medium speed|11: High speed
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;//[1:0]00: No pull-up, pull-down|01: Pull-up|10: Pull-down|11: Reserved
}

void vGPIOB(){
    //*****PB0 - Alternate_function_mode|output_push-pull|High_speed|pull_down*****
    GPIOB->MODER = 0x00000000;
    GPIOB->MODER &= ~GPIO_MODER_MODER0_0;
    GPIOB->MODER |= GPIO_MODER_MODER0_1;//[1:0]00: Input mode (reset state)|01: General purpose output mode|10: Alternate function mode|11: Analog mode
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_0;//[1:0]0: Output push-pull (reset state)|1: Output open-drain
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_0 | GPIO_OSPEEDER_OSPEEDR0_1;//[1:0]x0: Low speed|01: Medium speed|11: High speed
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0_0;//[1:0]00: No pull-up, pull-down|01: Pull-up|10: Pull-down|11: Reserved
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR0_1;//[1:0]00: No pull-up, pull-down|01: Pull-up|10: Pull-down|11: Reserved
    GPIOB->AFR[0] |= 2;//AF2 GPIOx_AFR
    //GPIOB->BSRR |= GPIO_BSRR_BR_0;
    //for(int i = 0; i<100000; i++){}
}