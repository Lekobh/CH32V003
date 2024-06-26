# PWM Control for CH32V00x with Frequency Adjustment

This project demonstrates the implementation of PWM (Pulse Width Modulation) control using Timer1 on a CH32V00x microcontroller. The code generates a PWM output with variable duty cycle and adjustable frequency on pin PC4, with a maximum frequency of approximately 40 kHz.

## Features

- Uses Timer1 of CH32V00x to generate the PWM signal
- PWM output on pin PC4 (TIM1_CH4)
- Variable duty cycle from 0% to 100%
- Adjustable PWM frequency through the PWMfreq constant (max ~40 kHz)
- Custom clock configuration for 48MHz system clock
- Serial communication for debugging and monitoring

## Hardware Requirements

- Development board with CH32V00x microcontroller
- 24MHz external crystal
- Serial connection for debugging (e.g., UART to USB)
- LED or oscilloscope connected to pin PC4 to visualize the PWM output

## How to Use

1. Compile and load the code onto your CH32V00x device
2. Connect an LED or oscilloscope to pin PC4
3. Open a serial terminal (115200 baud) to view debug messages
4. The LED connected to PC4 should blink with varying intensity, or you'll see a variable PWM signal on the oscilloscope

## Customization

You can adjust the following parameters in the code:

- `PWMstep`: Controls the speed of duty cycle change (default: 1)
- `PWMmax`: Defines the maximum value of the duty cycle (default: 100)
- `PWMfreq`: Sets the desired PWM signal frequency in Hz (default: 40000, max ~40 kHz)

## Operation

The code uses a main loop that gradually increases and decreases the PWM duty cycle. The direction of change is reversed when the duty cycle reaches 0% or 100%. The current duty cycle state is printed via UART every 500ms.

The PWM frequency is configured through the `PWMfreq` constant. The code automatically calculates the appropriate values for the timer prescaler and period based on the system frequency and the desired PWM frequency.

## Clock Configuration

The system clock is configured to run at 48MHz using the following setup:
- External 24MHz crystal (HSE) is enabled
- PLL is configured to multiply the HSE by 2
- System clock is set to use the PLL output

## PWM Frequency Calculation

The PWM frequency is calculated using the following formula:

```
PWM_frequency = timer_clock / ((prescaler + 1) * (period + 1))
```

Where:
- `timer_clock` is the system clock frequency (48MHz)
- `prescaler` and `period` are calculated in the code based on the desired `PWMfreq`

## Troubleshooting

If you're not seeing the PWM output or the frequency is incorrect:

1. Check if pin PC4 is correctly connected
2. Confirm that the external 24MHz crystal is properly connected and functioning
3. Use an oscilloscope to check if there's any signal on pin PC4 and measure its frequency
4. Check the debug messages on the serial output
5. Make sure the chosen `PWMfreq` is compatible with your microcontroller's capabilities (max ~40 kHz)
