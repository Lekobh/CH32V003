#include "ch32v00x.h"
#include "debug.h"


// Led D0  Interrupt C3
#define LED_PIN GPIO_Pin_0
#define INTERRUPT_PIN GPIO_Pin_3

volatile uint32_t interrupt_count = 0;

void GPIO_Config(void);


int main(void) {
    Delay_Init();
    USART_Printf_Init(9600);
    printf("GPIO Interrupt Test\r\n");

    GPIO_Config();

    while (1) {
        printf("Interrupt Count: %lu\r\n", interrupt_count);
        Delay_Ms(1000);
    }
}

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    EXTI_InitTypeDef EXTI_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // Configure LED pin
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // Configure interrupt pin
    GPIO_InitStructure.GPIO_Pin = INTERRUPT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Configure EXTI line
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Configure NVIC
    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      EXTI0_IRQHandler
 *
 * @brief   This function handles EXTI0 Handler.
 *
 * @return  none
 */
void EXTI7_0_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
        interrupt_count++;
        GPIO_WriteBit(GPIOD, LED_PIN, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOD, LED_PIN)));
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
