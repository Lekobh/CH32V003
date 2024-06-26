GPIO Interrupt Test with CH32V003
This project demonstrates how to configure and handle GPIO interrupts using the CH32V003 microcontroller. The example code sets up an interrupt on a GPIO pin and toggles an LED each time the interrupt is triggered.

Requirements
CH32V003 microcontroller

LED connected to pin D0

Interrupt signal connected to pin C3

UART for debugging and data output

Setup
Connect an LED to pin D0 of the CH32V003.

Connect the interrupt signal to pin C3 of the CH32V003.

Configure the UART for serial communication at 9600 baud rate.

Flash the provided code to the CH32V003 microcontroller.

Code Overview
The code is organized into the following main functions: main()The main() function initializes the system, including the GPIO and UART, and enters a loop where it continuously prints the interrupt count.

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
GPIO_Config()The GPIO_Config() function sets up the GPIO pins and configures the external interrupt.

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
EXTI7_0_IRQHandler()The EXTI7_0_IRQHandler() function handles the interrupt triggered by the external interrupt pin. It increments the interrupt count and toggles the LED.

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
Usage
Compile and flash the code to the CH32V003 microcontroller.

Open a serial terminal at 9600 baud rate.

Observe the interrupt count printed on the terminal and the LED toggling each time the interrupt is triggered.

Notes
Ensure the interrupt signal is properly connected and configured to trigger on the falling edge.

Adjust the interrupt priority and subpriority as needed based on the specific application requirements.

License
This project is licensed under the MIT License.

For more information and detailed documentation, please refer to the CH32V003 datasheet .