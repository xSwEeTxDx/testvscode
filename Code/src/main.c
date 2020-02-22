#include "main.h"

int main(){
    vRCC_Init();
   
    vGPIO_Init();
    vGPIOA();
    vGPIOB();
    vDMA1_Channel2_Mem_To_TIM3_Init();
    vTIM3_Init();
    
    
    

//    ws2812b_send();

    while (1)
    {

    }
    
}