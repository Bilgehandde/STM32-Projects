// 1. To use standard integer types (like uint32_t etc.)
#include <stdint.h>

// 2. Define Register Addresses
// These addresses are taken from the STM32F103 Reference Manual.

// RCC (Reset and Clock Control) Base Address
#define RCC_BASE      0x40021000
// APB2 peripheral clock enable register (RCC_APB2ENR)
// Task: To provide a clock signal to the GPIOC port.
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE + 0x18)

// GPIOC (General Purpose I/O Port C) Base Address
#define GPIOC_BASE    0x40011000
// Port C configuration register high (GPIOC_CRH) - (For Pins 8-15)
// Task: To configure the PC13 pin as 'output'.
#define GPIOC_CRH     *(volatile uint32_t *)(GPIOC_BASE + 0x04)
// Port C output data register (GPIOC_ODR)
// Task: To send a '0' (ON) or '1' (OFF) signal to the PC13 pin.
#define GPIOC_ODR     *(volatile uint32_t *)(GPIOC_BASE + 0x0C)


/*
 * Simple delay function (busy-wait).
 * It makes the processor wait idly for 'count' cycles.
 */
void delay_ms(volatile uint32_t count) {
    while (count--);
}

int main(void) {

    // --- Step 1: Enable GPIOC Port Clock Signal ---

    // We are setting the 4th bit, 'IOPCEN' (I/O Port C Clock Enable),
    // of the RCC_APB2ENR register to '1'.
    // (1 << 4) = 0b10000 = 0x10.
    RCC_APB2ENR |= (1 << 4);


    // --- Step 2: Configure PC13 Pin as 'Output' ---

    // Since PC13 is in the high pin group (8-15) of GPIOC, we use
    // the GPIOC_CRH register. The bits controlling PC13 are 20-23.

    // First, let's clear these 4 bits (Reset state)
    GPIOC_CRH &= ~(0x0F << 20); // (0b1111 << 20)

    // Now let's set PC13 as 'General Purpose Output Push-Pull'
    // and its speed to 50Mhz.
    // MODE13[1:0] = 11 (Output mode, max speed 50 MHz)
    // CNF13[1:0]  = 00 (General purpose output push-pull)
    // In total: (0b0011 << 20) = (0x3 << 20)
    GPIOC_CRH |= (0x03 << 20);

    // --- Step 3: Infinite Loop (LED Toggling) ---

    while (1) {
        // Toggle (invert) the state of the PC13 pin
        // We apply an XOR (^) operation to the 13th bit of the ODR register.
        // (If the bit is 0, it becomes 1; if 1, it becomes 0)
        GPIOC_ODR ^= (1 << 13);

        // NOTE: Adjust 1000000 to get a 500ms delay
        // (for a 1-second total period) on your specific board.
        delay_ms(1000000);
    }
}
