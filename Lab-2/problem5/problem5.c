#include <stdint.h>

// --- RCC (Reset and Clock Control) ---
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE + 0x18)

// --- GPIOA (For LEDs, PA0-PA7) ---
#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOA_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x0C)

void delay_ms(volatile uint32_t count) {
    while (count--);
}


int main(void) {

    // --- Step 1: Enable Clock Signals ---

    // Enable clock for GPIOA (LEDs) (Bit 2: IOPAEN)
    RCC_APB2ENR |= (1 << 2);

    // --- Step 2: Configure Pins (LEDs - PA0-PA7) ---

    // Set pins PA0-PA7 as 'Output' (Same as Problem 3)
    GPIOA_CRL = 0x33333333;

    // --- Step 3: Initialize Variables and Loop ---

    // LED pattern (uint8_t = 8-bit integer)
    // Initial state: LED1, LED2, LED3 (PA0, PA1, PA2) are ON
    uint8_t led_pattern = 0b00000111; // (PA0, PA1, PA2 = 1)

    // Direction state:
    // 0 = Shift Left (<<) (Direction PA0 -> PA7)
    // 1 = Shift Right (>>) (Direction PA7 -> PA0)
    int direction = 0;

    while (1) {

        // --- 3a. Write the Pattern to the LEDs ---
        // Update only the first 8 bits (PA0-PA7) of GPIOA_ODR
        GPIOA_ODR = (GPIOA_ODR & 0xFFFFFF00) | led_pattern;

        // --- 3b. Delay ---
        // The lab sheet asks for ~100 ms delay
        // YOU MUST adjust this value (100000) to get 100ms.
        delay_ms(100000);

        // --- 3c. Update Pattern and Direction ---
        if (direction == 0) {
            // Direction = Shift Left (<<), towards LED8 (PA7)

            // If we have reached the right edge (PA5, PA6, PA7):
            if (led_pattern == 0b11100000) {
                direction = 1; // Reverse direction (Shift Right)
                led_pattern = led_pattern >> 1; // Start shifting right (0b01110000)
            }
            else {
                // If we haven't reached the edge, keep shifting left
                led_pattern = led_pattern << 1;
            }
        }
        else {
            // Direction = Shift Right (>>), towards LED1 (PA0)
            // If we have reached the left edge (PA0, PA1, PA2):
            if (led_pattern == 0b00000111) {
                direction = 0; // Reverse direction (Shift Left)
                led_pattern = led_pattern << 1; // Start shifting left (0b00001110)
            }
            else {
                // If we haven't reached the edge, keep shifting right
                led_pattern = led_pattern >> 1;
            }
        }
    }
}
