#include <stdbool.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


// Current state: 
// https://microtechnics.ru/stm32-uchebnyj-kurs-gpio-porty-vvoda-vyvoda/

void initAll(){
    GPIO_InitTypeDef port;
    
    // Функция включания тактирования порта GPIOA:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // PA1 - для отрабоки нажатия кнопки:
    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_IPD;
    port.GPIO_Pin = GPIO_Pin_1;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &port);
    
    // PA0 - для светодиода
    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_Out_PP;
    port.GPIO_Pin = GPIO_Pin_0;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &port);
}

int main(){
    uint8_t button_state = 0;
    initAll();
    while(1){
        button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
        if (button_state == 1){
            GPIO_SetBits (GPIOA, GPIO_Pin_0);
        }
        else{
            GPIO_ResetBits (GPIOA, GPIO_Pin_0);
        }
    }
}


// Previous version:
/*
void PORTB_5_INIT(void);

void PORTB_5_INIT (void){
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // разрешаем тактирование порта B
    
    GPIOB -> CRL &= ~GPIO_CRL_MODE5_0;
    GPIOB -> CRL |= GPIO_CRL_MODE5_1;
    GPIOB -> CRL &= ~GPIO_CRL_CNF5;
}


int main(void){
    
    
    PORTB_5_INIT();
    
    while(1){
        GPIOA->BSRR |= GPIO_BSRR_BS5;
        GPIOA->BSRR |= GPIO_BSRR_BR5;
}

}
*/
