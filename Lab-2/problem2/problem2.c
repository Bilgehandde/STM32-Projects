#include <stdint.h>

// --- RCC (Reset and Clock Control) ---
#define RCC_BASE      0x40021000
// APB2 peripheral clock enable register
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE + 0x18)

// --- GPIOC (For the LED) ---
#define GPIOC_BASE    0x40011000
// Port C configuration register high (Pin 8-15)
#define GPIOC_CRH     *(volatile uint32_t *)(GPIOC_BASE + 0x04)
// Port C output data register
#define GPIOC_ODR     *(volatile uint32_t *)(GPIOC_BASE + 0x0C)

// --- GPIOA (ADDED for the Button) ---
#define GPIOA_BASE    0x40010800
// Port A configuration register low (Pin 0-7)
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)
// Port A input data register (To read the button state)
#define GPIOA_IDR     *(volatile uint32_t *)(GPIOA_BASE + 0x08)
// Port A output data register (For Pull-down selection)
#define GPIOA_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x0C)

int main(void) {

    // --- Step 1: Enable Clock Signals ---

    // Enable clock signal for GPIOC (LED) (Bit 4: IOPCEN)
    RCC_APB2ENR |= (1 << 4);

    // Enable clock signal for GPIOA (Button) (Bit 2: IOPAEN) - NEW
    RCC_APB2ENR |= (1 << 2);


    // --- Step 2: Configure Pins (LED) ---

    // Configure PC13 (On-board LED) as 'Output' (Same as Problem 1)
    GPIOC_CRH &= ~(0x0F << 20); // Clear
    GPIOC_CRH |= (0x03 << 20);  // 50Mhz General Purpose Output

    // Turn off the LED initially (PC13 = High)
    GPIOC_ODR |= (1 << 13);


    // --- Step 3: Configure Pins (Button) - NEW ---

    // Configure PA0 (Button) pin as 'Input with Pull-down'
    // PA0 is in the low pin group (0-7) of GPIOA, so we use GPIOA_CRL
    // register. Bits 0-3 control PA0.

    // First, let's clear these 4 bits
    GPIOA_CRL &= ~(0x0F << 0); // (0b1111 << 0)

    // Now let's set PA0 as 'Input with pull-up/pull-down'
    // MODE0[1:0] = 00 (Input mode)
    // CNF0[1:0]  = 10 (Input with pull-up/pull-down)
    // In total: (0b1000 << 0) = (0x8 << 0)
    GPIOA_CRL |= (0x08 << 0);

    // Let's select the pull-down resistor (we would set to '1' for pull-up)
    // We select PULL-DOWN for PA0 by setting the 0th bit of the ODR register to '0'.
    GPIOA_ODR &= ~(1 << 0);

    // --- Step 4: Infinite Loop (Read Button) ---
    while (1) {

        // Read the PA0 pin (0th bit of IDR)
        if ((GPIOA_IDR & (1 << 0)) != 0) {
            // This means the button is pressed (Pin becomes '1' (High) from 3.3V)
            // Turn the LED ON (Send '0' (Low) to PC13)
            GPIOC_ODR &= ~(1 << 13);
        }
        else {
            // This means the button is not pressed (Pin is '0' (Low) via pull-down)
            // Turn the LED OFF (Send '1' (High) to PC13)
            GPIOC_ODR |= (1 << 13);
        }
    }
}
