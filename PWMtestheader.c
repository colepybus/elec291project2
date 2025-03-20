// Turn a pin high and low to test PWM 
#include "stm32l051xx.h"

#define PWM_FREQ 100  // PWM frequency in Hz
#define SYSCLK 2000000 // System clock frequency in Hz (2 MHz for low power STM32L0)
#define TIMER_RELOAD (SYSCLK / (PWM_FREQ * 100))

volatile uint8_t duty_cycle1 = 50; // Default 50% duty cycle
volatile uint8_t duty_cycle2 = 50;

void TIM2_IRQHandler(void) {
    static uint8_t count = 0;
    count = (count + 1) % 100;
    
    if (count < duty_cycle1)
        GPIOA->ODR |= (1 << 4);  // Set PA4 high
    else
        GPIOA->ODR &= ~(1 << 4); // Set PA4 low
    
    if (count < duty_cycle2)
        GPIOA->ODR |= (1 << 5);  // Set PA5 high
    else
        GPIOA->ODR &= ~(1 << 5); // Set PA5 low
    
    TIM2->SR &= ~TIM_SR_UIF; // Clear update interrupt flag
}

void USART2_IRQHandler(void) {
    if (USART2->ISR & USART_ISR_RXNE) {
        duty_cycle1 = USART2->RDR; // Read first duty cycle
        while (!(USART2->ISR & USART_ISR_RXNE));
        duty_cycle2 = USART2->RDR; // Read second duty cycle
    }
}

void Timer2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock
    TIM2->PSC = (SYSCLK / 10000) - 1;   // Prescaler for 10 kHz timer clock
    TIM2->ARR = 100;                    // 100 Hz PWM cycle
    TIM2->DIER |= TIM_DIER_UIE;         // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;           // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn);          // Enable TIM2 interrupt in NVIC
}

void UART2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Enable USART2 clock
    GPIOA->MODER |= (2 << (2 * 2)) | (2 << (3 * 2)); // PA2, PA3 in AF mode
    GPIOA->AFR[0] |= (4 << (2 * 4)) | (4 << (3 * 4)); // AF4 for USART2
    USART2->BRR = SYSCLK / 9600;          // Baud rate 9600
    USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;
    NVIC_EnableIRQ(USART2_IRQn);          // Enable USART2 interrupt
}

void GPIO_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable GPIOA clock
    GPIOA->MODER |= (1 << (4 * 2)) | (1 << (5 * 2)) | (1 << (6 * 2)) | (1 << (7 * 2)); // Set PA4-PA7 as output
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (SYSCLK / 8000) * ms; i++) {
        __NOP(); // No operation (delay loop)
    }
}

int main(void) {
    GPIO_Init();
    Timer2_Init();  // Initialize timer 2
    UART2_Init();
    __enable_irq(); // Enable global interrupts
    
    while (1) {
        GPIOA->ODR ^= (1 << 4); // Toggle PA4
        GPIOA->ODR ^= (1 << 6); // Toggle PA6
        delay_ms(500);          // 500 ms delay
    }
}
