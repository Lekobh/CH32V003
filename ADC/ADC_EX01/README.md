

# ADC and Temperature Reading with CH32V003 

This project demonstrates how to read ADC values and convert them to temperature using an NTC thermistor with the CH32V003 microcontroller. The example code initializes the ADC, reads the ADC value, and converts it to temperature using a lookup table.

## Requirements 

- CH32V003 microcontroller

- NTC thermistor: NTC  10k b = 3435 +-1%  -40 c ~ 120 c.

- Resistor 2K2 (pulldown of NTC).

- UART for debugging and data output

## Setup 

1. Connect the NTC thermistor to the appropriate ADC channel on the CH32V003.

2. Configure the UART for serial communication at 115200 baud rate.

3. Flash the provided code to the CH32V003 microcontroller.

## Code Overview 

The code is organized into the following main functions:
`main()`The `main()` function initializes the system, including the ADC and UART, and enters a loop where it continuously reads the ADC value, converts it to temperature, and prints the results.

```c
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
```
`InitializeADC()`The `InitializeADC()` function sets up the ADC for temperature readings on the specified channel.

```c
void InitializeADC(void) {
    RCC->APB2PCENR |= RCC_APB2Periph_ADC1;
    
    ADC1->CTLR1 = 0;
    ADC1->CTLR2 = ADC_ADON | ADC_EXTSEL;
    ADC1->SAMPTR2 = ADC_SampleTime_241Cycles << (TEMPERATURE_CHANNEL * 3);
    ADC1->RSQR1 = 0;
    ADC1->RSQR3 = TEMPERATURE_CHANNEL;

    ADC1->CTLR2 |= ADC_ADON; // Start ADC
}
```
`ConvertADCToTemperature()`The `ConvertADCToTemperature()` function converts the ADC value to temperature using a predefined lookup table.

```c
int ConvertADCToTemperature(uint16_t adc_value) {
    int p1 = NTC_table[adc_value >> 5];
    int p2 = NTC_table[(adc_value >> 5) + 1];
    return p1 + ((p2 - p1) * (adc_value & 0x001F)) / 32;
}
```

## NTC Lookup Table 
The NTC lookup table `NTC_table` contains temperature values corresponding to ADC readings.

```c
const int NTC_table[33] = {
    -314, -170, -26, 69, 144, 208, 264, 315,
    363, 408, 452, 494, 536, 577, 618, 660, 702,
    746, 791, 838, 887, 940, 997, 1058, 1127,
    1204, 1294, 1401, 1534, 1714, 1986, 2521,
    3056
};
```

## Usage 

1. Compile and flash the code to the CH32V003 microcontroller.

2. Open a serial terminal at 115200 baud rate.

3. Observe the ADC values and corresponding temperature readings printed on the terminal.

## Notes 

- Ensure the NTC thermistor is properly connected and calibrated for accurate temperature readings.

- Adjust the lookup table and sampling time as needed based on the specific characteristics of the NTC thermistor used.

## License 

This project is licensed under the MIT License.


---

For more information and detailed documentation, please refer to the [CH32V003 datasheet]() .
