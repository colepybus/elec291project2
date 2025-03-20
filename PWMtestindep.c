#define PWM_FREQ 100  // PWM frequency in Hz
#define SYSCLK 2000000 // System clock frequency in Hz (2 MHz for low power STM32L0)
#define TIMER_RELOAD (SYSCLK / (PWM_FREQ * 100))

#define RCC_APB1ENR_TIM2EN (1 << 0)
#define RCC_APB1ENR_USART2EN (1 << 17)
#define RCC_AHBENR_GPIOAEN (1 << 17)
#define TIM_CR1_CEN (1 << 0)
#define TIM_DIER_UIE (1 << 0)
#define TIM_SR_UIF (1 << 0)
#define USART_CR1_RE (1 << 2)
#define USART_CR1_TE (1 << 3)
#define USART_CR1_RXNEIE (1 << 5)
#define USART_CR1_UE (1 << 0)
#define USART_ISR_RXNE (1 << 5)

volatile uint8_t duty_cycle1 = 50; // Default 50% duty cycle
volatile uint8_t duty_cycle2 = 50;

void TIM2_IRQHandler(void) {
    static uint8_t count = 0;
    count = (count + 1) % 100;
    
    if (count < duty_cycle1)
        *((volatile unsigned int*)0x50000014) |= (1 << 0);  // Set PA0 high
    else
        *((volatile unsigned int*)0x50000014) &= ~(1 << 0); // Set PA0 low
    
    if (count < duty_cycle2)
        *((volatile unsigned int*)0x50000014) |= (1 << 1);  // Set PA1 high
    else
        *((volatile unsigned int*)0x50000014) &= ~(1 << 1); // Set PA1 low
    
    *((volatile unsigned int*)0x40000010) &= ~TIM_SR_UIF; // Clear update interrupt flag
}

void USART2_IRQHandler(void) {
    if (*((volatile unsigned int*)0x4000441C) & USART_ISR_RXNE) {
        duty_cycle1 = *((volatile unsigned int*)0x40004424); // Read first duty cycle
        while (!(*((volatile unsigned int*)0x4000441C) & USART_ISR_RXNE));
        duty_cycle2 = *((volatile unsigned int*)0x40004424); // Read second duty cycle
    }
}

void Timer2_Init(void) {
    *((volatile unsigned int*)0x40021058) |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock
    *((volatile unsigned int*)0x40000028) = (SYSCLK / 10000) - 1; // Prescaler for 10 kHz timer clock
    *((volatile unsigned int*)0x4000002C) = 100; // 100 Hz PWM cycle
    *((volatile unsigned int*)0x4000000C) |= TIM_DIER_UIE; // Enable update interrupt
    *((volatile unsigned int*)0x40000000) |= TIM_CR1_CEN; // Enable timer
}

void UART2_Init(void) {
    *((volatile unsigned int*)0x40021058) |= RCC_APB1ENR_USART2EN; // Enable USART2 clock
    *((volatile unsigned int*)0x4000440C) = SYSCLK / 9600; // Baud rate 9600
    *((volatile unsigned int*)0x4000440C) |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;
}

void GPIO_Init(void) {
    *((volatile unsigned int*)0x40021014) |= RCC_AHBENR_GPIOAEN; // Enable GPIOA clock
    *((volatile unsigned int*)0x50000000) |= (1 << (0 * 2)) | (1 << (1 * 2)); // Set PA0, PA1 as output
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < (SYSCLK / 8000) * ms; i++) {
        __asm("NOP"); // No operation (delay loop)
    }
}

int main(void) {
    GPIO_Init();
    Timer2_Init();
    UART2_Init();
    __asm("CPSIE i"); // Enable global interrupts
    
    while (1) {
        *((volatile unsigned int*)0x50000014) ^= (1 << 0); // Toggle PA0
        delay_ms(500);          // 500 ms delay
    }
}
