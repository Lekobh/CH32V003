#include <stdint.h>
#include "ch32v00x.h"
//#include "debug.h"

#define ADC_RESOLUTION 1024
#define TEMPERATURE_CHANNEL 2

const int NTC_table[33] = {
    -314, -170, -26, 69, 144, 208, 264, 315,
    363, 408, 452, 494, 536, 577, 618, 660, 702,
    746, 791, 838, 887, 940, 997, 1058, 1127,
    1204, 1294, 1401, 1534, 1714, 1986, 2521,
    3056
};

void InitializeADC(void);
int ConvertADCToTemperature(uint16_t adc_value);

int main(void) {
    Delay_Init();
    USART_Printf_Init(115200);
    printf("ADC and Temperature Reading Test\r\n");

    InitializeADC();

    while (1) {
        ADC1->CTLR2 |= ADC_SWSTART; // Start conversion
        while (!(ADC1->STATR & ADC_EOC)); // Wait for conversion to complete

        uint16_t adc_value = ADC1->RDATAR;
        int temperature = ConvertADCToTemperature(adc_value);

        printf("ADC Value: %d, Temperature: %d.%d C\r\n",
               adc_value, temperature / 10, abs(temperature % 10));

        Delay_Ms(1000); // Wait for 1 second before next reading
    }
}

void InitializeADC(void) {
    RCC->APB2PCENR |= RCC_APB2Periph_ADC1;

    ADC1->CTLR1 = 0;
    ADC1->CTLR2 = ADC_ADON | ADC_EXTSEL;
    ADC1->SAMPTR2 = ADC_SampleTime_241Cycles << (TEMPERATURE_CHANNEL * 3);
    ADC1->RSQR1 = 0;
    ADC1->RSQR3 = TEMPERATURE_CHANNEL;

    ADC1->CTLR2 |= ADC_ADON; // Start ADC
}

int ConvertADCToTemperature(uint16_t adc_value) {
    int p1 = NTC_table[adc_value >> 5];
    int p2 = NTC_table[(adc_value >> 5) + 1];
    return p1 + ((p2 - p1) * (adc_value & 0x001F)) / 32;
}
