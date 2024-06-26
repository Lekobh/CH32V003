#include "ch32v00x.h"
#include "debug.h"

#define PWM_PIN GPIO_Pin_4

void confClock (void);
void Timer1_PWM_Init(void);

const uint16_t PWMstep = 1;
const uint16_t PWMmax = 100;
const uint32_t PWMfreq = 40000; // Frequência desejada em Hz max ~40K

int main(void) {
    confClock ();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("PWM Test using Timer1\r\n");
    Timer1_PWM_Init();

    uint16_t duty_cycle = 0;
    uint8_t direction = 1;

    while (1) {
        TIM_SetCompare4(TIM1, duty_cycle);
        if (direction) {
            duty_cycle += PWMstep;
            if (duty_cycle >= PWMmax) direction = 0;
        } else {
            duty_cycle -= PWMstep;
            if (duty_cycle == 0) direction = 1;
        }
        printf("Duty Cycle: %d/%d \r\n", duty_cycle, PWMmax);
        Delay_Ms(500);
    }
}

void Timer1_PWM_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = PWM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Calcular o prescaler e o período para a frequência desejada
    uint32_t timer_clock = SystemCoreClock;
    uint16_t prescaler = (uint16_t)((uint16_t)(timer_clock/(PWMfreq+1))/(PWMmax));
    uint16_t period = (PWMmax - 1);

    TIM_TimeBaseInitStructure.TIM_Period = period;
    TIM_TimeBaseInitStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}
void confClock (void){
    //Configura Clocks
            RCC_HSEConfig(RCC_HSE_ON);                  //Cristal externo 24MHz
            RCC_WaitForHSEStartUp();
            RCC_PLLConfig(RCC_PLLSource_HSE_MUL2);      //coloca pll no clock externo
            RCC_PLLCmd(ENABLE);
            RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //configura o clock do systema para o pll
            RCC_HCLKConfig(RCC_SYSCLK_Div1);            //esta em 48MHz
}
