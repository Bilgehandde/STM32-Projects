#include <stdint.h>

// --- RCC (Reset and Clock Control) ---
#define RCC_BASE      0x40021000
// APB2 peripheral clock enable register
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE + 0x18)

// --- GPIOA (For 8 external LEDs) ---
#define GPIOA_BASE    0x40010800
// Port A configuration register low (For Pins 0-7)
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)
// Port A output data register
#define GPIOA_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x0C)

/*
 * Simple delay function (busy-wait).
 */
void delay_ms(volatile uint32_t count) {
    while (count--);
}

int main(void) {

    // --- Step 1: Enable GPIOA Port Clock Signal ---

    // Enable clock signal for GPIOA (LEDs) (Bit 2: IOPAEN)
    RCC_APB2ENR |= (1 << 2);

    // --- Step 2: Configure Pins (PA0 - PA7) ---

    // We must configure all 8 pins from PA0 to PA7 as 'Output'.
    // All these pins are controlled by the GPIOA_CRL register.

    // Setting for each pin: 0b0011 (General Purpose Output 50Mhz)
    // For 8 pins: 0x33333333

    // Configure all 8 pins at once by writing this value to the entire GPIOA_CRL register
    GPIOA_CRL = 0x33333333;

    // --- Step 3: Infinite Loop (Toggle all LEDs) ---
    while (1) {

        // Toggle (invert) the first 8 bits of GPIOA_ODR (0x00FF = 0b11111111).
        GPIOA_ODR ^= 0x00FF;

        // Delay
        // NOTE: Adjust 1000000 to get a 500ms delay
        // (for a 1-second total period) on your specific board.
        delay_ms(1000000);
    }
}
