// Testing CMSIS programs:

/*

#include "stm32f10x_gpio.h" // файл с функциями управления ножками мк
#include "stm32f10x_rcc.h"  // управление тактированием
#include <stdbool.h>
#include "stm32f10x.h"

#define LEDPORT (GPIOC)
#define LED1 (13)

// Разрешение тактирования порта C
#define ENABLE_GPIO_CLOCK (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)


// Разрешение работы пина 13
#define GPIOMODER (GPIO_CRH_MODE13_0)



// #include "system_stm32f10x.h"

// Block of functions:
void PORTB_5_INIT (void);
void pause_MCU (void);
void msDelay(int ms);

void msDelay(int ms)
{

   while (ms-- > 0) {
      volatile int x=5000;
      while (x-- > 0)
         __asm("nop");
   }
}

void pause_MCU (void){
    for ( uint16_t i = 0; i <= 500; i++){
        for (uint16_t j = 0; j <= 2; j++);
    }
}

void PORTB_5_INIT (void){
        RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
        
        GPIOB -> CRL &= ~GPIO_CRL_MODE5_0;
        GPIOB -> CRL |= ~GPIO_CRL_MODE5_1;    
        GPIOB -> CRL &= ~GPIO_CRL_CNF5;
}

int main(void){
        // PORTB_5_INIT ();
    
        // Включаем тактирование порта GPIOC:
        // RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE); // SPL
        RCC -> APB2ENR |= (1 << 4); // CMSIS - 146
    
        // Инициалищируем ногу со светодиодом (PC13)
        GPIOC->CRH |= (0x00 << 22) | (0x00 << 20); // CMSIS - 172
    
        while(1){
                //GPIOB -> BSRR |= GPIO_BSRR_BS5;
                //GPIOB -> BSRR |= GPIO_BSRR_BR5;  
        GPIOC->BSRR = (1 << 13); // устанавливаем PC13
        pause_MCU();
        //msDelay(3000);
            
        GPIOC->BSRR = (1 << 29); // сбрасываем PC13
        pause_MCU();
        //msDelay(5000);
            
        }
}
*/


/* 

// Working one:
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"     // Файл с функциями управления ножками контроллера
#include "stm32f10x_rcc.h"      // Управление тактированием
#include <stdint.h>             // Правильные типы данных, вместо всяких int, char и тому подобных

// Далее будут ссылки на страницы референс мануала, его можно сказать по ссылке:
// https://www.st.com/resource/en/reference_manual/CD00171190.pdf

// Примитивнейшая функция задержки
// Для орагнизации задержек (или периодических событий)
// обычно применяются таймеры, но нам пока рано, так что
// тупо мотаем такты

void Delay(void);

void Delay( void )
{
    for( uint16_t i = 0; i <= 5000; i++ );
       // for( uint16_t j = 0; j <= 10; j++ );
}

// точка входа в программу, всегда именно "int main ( void )"
int main( void )
{

    // ---------------- Включаем тактирование порта GPIOC
    // 1. С использованием SPL
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );  

    // 2. С использованием CMSIS, магические числа
//    RCC->APB2ENR |= (1 << 4);                       // см 146 стр. 
    // ---------------- 

    // ---------------- Инициализируем ногу со светодиодом (PC13 - Порт С нога 13)    
    // 1. С использованием SPL
    GPIO_InitTypeDef PortC;                     // Структура с необходимыми полями
    PortC.GPIO_Mode         = GPIO_Mode_Out_PP;     // Выход пуш-пул, см 164 стр.
    PortC.GPIO_Speed        = GPIO_Speed_10MHz;     // По сути это ток который сможет обеспечить вывод
    PortC.GPIO_Pin          = GPIO_Pin_13;          // Номер ноги
    GPIO_Init(GPIOC, &PortC);                       // Применяем настройки

    // 2. С использованием CMSIS
//    GPIOC->CRH |= (0x00 << 22) | (0x01 << 20);      // см 172 стр. 
    // ---------------- 

    // Основной цикл, программа ВСЕГДА должна зацикливаться!!!
    // Не всегда наполнен чем-то вразумительным, иногда может быть пустым, 
    // например когда вся логика реализована в прерываниях.
    while( 1 )
    {
        // ---------------- Устанавливае ногу со светодиодом (PC13 - Порт С нога 13)
        // 1. С использованием SPL
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);     
        // 2. С использованием CMSIS и ODR регистра, см 173 стр.
//        GPIOC->ODR |= (1 << 13);                     
        // 3. C использованием CMSIS и BSRR регистра, см 173 стр.
//        GPIOC->BSRR = (1 << 13);                        
        // ---------------- 

        Delay();

        // ---------------- Сбрасываем ногу со светодиодом (PC13 - Порт С нога 13)
        // 1. С использованием SPL
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);   
        // 2. С использованием CMSIS и ODR регистра, см 173 стр.
        // GPIOC->ODR &= ~(1 << 13);                       
        // 3. С использованием CMSIS и BRR регистра , см 174 стр.
        // GPIOC->BRR = (1 << 13);         
        // 4. С использованием CMSIS и BSRR регистра , см 173 стр.
        // GPIOC->BSRR = (1 << 29);                 
        // ---------------- 

        Delay();        
    }
}
// В конце файла для Кейла обязательна пустая строка! Хз зачем, просто нужна.
*/


#include <stdbool.h>
#include "stm32f10x.h"

#define LEDPORT (GPIOC)
#define LED1 (13)

// Разрешение тактирования порта C
#define ENABLE_GPIO_CLOCK (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)

// Разрешение работы пина 13
#define GPIOMODER (GPIO_CRH_MODE13_0)

#define _MODER CRH
void msDelay(int ms);
void msDelay(int ms)
{
   while (ms-- > 0) {
      volatile int x=4000;
      while (x-- > 0)
         __asm("nop");
   }
}

int main(void)
{
    // Enable the clock to GPIO
    ENABLE_GPIO_CLOCK;

    // Set pins to be general purpose output
    LEDPORT->_MODER |= GPIOMODER;
    while (1) {
        msDelay(5000);
        LEDPORT->ODR ^= (1<<LED1); // Toggle LED
    }
    //return 0;
}
