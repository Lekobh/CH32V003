# Chapter 1: Memory and Bus Architecture

## 1.1 Bus Architecture

The CH32V003 series is designed based on the RISC-V instruction set. Its architecture interacts with the core, arbitration unit, DMA module, SRAM storage, and other parts through multiple buses.

### System Block Diagram (textual representation)

```
1. Core Components:
   - RISC-V (V2A)
   - PFIC RV32EC
   - 1-wire SDI

2. Memory:
   - Flash Memory (connected to Flash CTRL)
   - SRAM

3. Buses:
   - I-code Bus
   - D-code Bus
   - System Bus

4. Clock Sources:
   - LSI-RC
   - HSE
   - HSI-RC

5. Peripherals:
   - DMA (7 Channels)
   - TIM1 (4 channels, 3 complementary Channels, ETR, BIKN)
   - USART (RX, TX, CTS, RTS, CK)
   - I2C (SCL, SDA)
   - SPI (MOSI, MISO, SCK, NSS)
   - ADC (AIN0~AIN7)
   - GPIO (GPIOA: PA1~PA2, GPIOC: PC0~PC7, GPIOD: PD0~PD7)
   - TIM2 (4 channels, ETR)
   - IWDG
   - WWDG
   - PWR
   - EXTI
   - AFIO
   - OPA (OPAPx, OPANx (x=0,1), OPAO)

6. Other Components:
   - Reset & Clock Control
   - SWIO
   - Power Supply (VDD: 2.7V~5.5V, VSS)
```

## 1.2 Memory Image

The CH32V003 family contains program memory, data memory, core registers, peripheral registers, and more, all addressed in a 4GB linear space.

### Memory Map (textual representation)

```
Memory Range              | Description
--------------------------|-----------------------------
0x00000000 - 0x07FFFFFF   | Aliased to Flash or system memory (depending on software configuration)
0x08000000 - 0x08003FFF   | Code FLASH (16KB)
0x08004000 - 0x1FFFF77F   | Reserved
0x1FFFF780 - 0x1FFFF7BF   | Vendor Bytes (64B)
0x1FFFF7C0 - 0x1FFFF7FF   | Option Bytes (64B)
0x1FFFF800 - 0x1FFFF83F   | Reserved
0x1FFFF840 - 0x1FFFFFF    | System FLASH (BOOT_1920B)
0x20000000 - 0x200007FF   | 2KB SRAM
0x20000800 - 0x3FFFFFFF   | Reserved
0x40000000 - 0x400023FF   | Peripherals (various)
0x40023800 - 0x4FFFFFFF   | Reserved
0x50050400 - 0x50050403   | EXTEND
0x50050404 - 0xDFFFFFFF   | Reserved
0xE0000000 - 0xE00FFFFF   | Core Private Peripherals
0xE0100000 - 0xFFFFFFFF   | Reserved

Peripheral Addresses (partial list):
0x40000000 - WWDG
0x40002C00 - IWDG
0x40007000 - PWR
0x40010000 - AFIO
0x40010400 - EXTI
0x40010800 - GPIOA
0x40010C00 - GPIOC
0x40012C00 - TIM1
0x40013000 - SPI
0x40020000 - DMA
0x40021000 - RCC
0x40005400 - I2C
0x40013800 - USART
0x40000000 - TIM2
0x40012400 - ADC
```

### 1.2.1 Memory Allocation

- Built-in 2KB SRAM, starting address 0x20000000, supports byte, half-word (2 bytes), and full-word (4 bytes) access.
- Built-in 16KB program Flash memory (CodeFlash) for storing user applications.
- Built-in 1920B System memory (bootloader) for storing the system bootloader (factory-cured bootloader).
- Built-in 64B space for vendor configuration word storage, factory-cured and unmodifiable by users.
- Built-in 64B space for user-option bytes storage.

