/* Motor Control Functions Source File
 * Implements the manual and automatic functions for DC motor control.
 * ELEC 291 Project 2
 * Group B01
 * Created March 26th, 2024 by Madison Howitt
 */

#include "motor_control.h"

volatile int PWM_Counter = 0;
volatile unsigned char pwm1 = 0, pwm2 = 0;

void move_backward(int speed) {
    GPIOA->ODR &= ~(BIT1 | BIT3);
    GPIOA->ODR |= (BIT0 | BIT2);
    pwm1 = speed;
    pwm2 = speed;
}

void move_forward(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2);
    GPIOA->ODR |= (BIT1 | BIT3);
    pwm1 = speed;
    pwm2 = speed;
}

void move_left(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2);
    GPIOA->ODR |= (BIT1 | BIT3);
    pwm1 = speed;
    pwm2 = 0;
}

void move_right(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT2);
    GPIOA->ODR |= (BIT1 | BIT3);
    pwm1 = 0;
    pwm2 = speed;
}

void move_stop(void) {
    pwm1 = 0;
    pwm2 = 0;
}

void wait_1ms(void) {
    SysTick->LOAD = (F_CPU/1000L) - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & BIT16) == 0);
    SysTick->CTRL = 0x00;
}

void delayms(int len) {
    while (len--) wait_1ms();
}

void TIM2_Handler(void) {
    TIM2->SR &= ~BIT0;
    PWM_Counter++;
    
    if (PWM_Counter < pwm1) {
        if (GPIOA->ODR & BIT1) {
            GPIOA->ODR |= BIT1;
        } else {
            GPIOA->ODR |= BIT0;
        }
    } else {
        GPIOA->ODR &= ~(BIT0 | BIT1);
    }
    
    if (PWM_Counter < pwm2) {
        if (GPIOA->ODR & BIT3) {
            GPIOA->ODR |= BIT3;
        } else {
            GPIOA->ODR |= BIT2;
        }
    } else {
        GPIOA->ODR &= ~(BIT2 | BIT3);
    }
    
    if (++PWM_Counter >= PWM_MAX) PWM_Counter = 0;
}

void Hardware_Init(void) {
    RCC->IOPENR |= BIT0;
    RCC->APB1ENR |= BIT0;

    GPIOA->MODER &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    GPIOA->MODER |= (BIT0 | BIT2 | BIT4 | BIT6);
    GPIOA->OTYPER &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    GPIOA->ODR &= ~(BIT0 | BIT1 | BIT2 | BIT3);

    RCC->APB1ENR |= BIT0;
    TIM2->ARR = F_CPU/DEF_F-1;
    NVIC->ISER[0] |= BIT15;
    TIM2->CR1 |= BIT4;
    TIM2->CR1 |= BIT7;
    TIM2->DIER |= BIT0;
    TIM2->CR1 |= BIT0;

    __enable_irq();
}

void turn_CW(int speed) {
    GPIOA->ODR &= ~(BIT1 | BIT2);
    GPIOA->ODR |= (BIT0 | BIT3);
    pwm1 = speed;
    pwm2 = speed;
}

void turn_CCW(int speed) {
    GPIOA->ODR &= ~(BIT0 | BIT3);
    GPIOA->ODR |= (BIT1 | BIT2);
    pwm1 = speed;
    pwm2 = speed;
}

void turn_random() {
    int turn_angle = (rand() % (180 - LOWER_ANGLE + 1)) + LOWER_ANGLE;
    int direction = rand() % 2;
    float turn_time = 0.88 * turn_angle / 180.0;

    if (direction == 1) {
        turn_CW(PWM_MAX);
    } else {
        turn_CCW(PWM_MAX);
    }
    delayms((int)(turn_time * 1000));
    move_forward(PWM_MAX);
}
