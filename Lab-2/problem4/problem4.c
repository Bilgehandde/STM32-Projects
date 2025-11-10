#include <stdint.h>

// --- RCC (Reset and Clock Control) ---
#define RCC_BASE      0x40021000
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE + 0x18)

// --- GPIOA (For LEDs, PA0-PA7) ---
#define GPIOA_BASE    0x40010800
#define GPIOA_CRL     *(volatile uint32_t *)(GPIOA_BASE + 0x00)
#define GPIOA_ODR     *(volatile uint32_t *)(GPIOA_BASE + 0x0C)

// --- GPIOB (For Button, PB0) ---
#define GPIOB_BASE    0x40010C00
#define GPIOB_CRL     *(volatile uint32_t *)(GPIOB_BASE + 0x00)
#define GPIOB_IDR     *(volatile uint32_t *)(GPIOB_BASE + 0x08)
#define GPIOB_ODR     *(volatile uint32_t *)(GPIOB_BASE + 0x0C)

void delay_ms(volatile uint32_t count) {
    while (count--);
}

int main(void) {

    // --- Step 1: Enable Clock Signals ---

    // Enable clock for GPIOA (LEDs) (Bit 2: IOPAEN)
    RCC_APB2ENR |= (1 << 2);

    // Enable clock for GPIOB (Button) (Bit 3: IOPBEN)
    RCC_APB2ENR |= (1 << 3);

    // --- Step 2: Configure Pins (LEDs - PA0-PA7) ---

    // Set pins PA0-PA7 as 'Output' (Same as Problem 3)
    GPIOA_CRL = 0x33333333;

    // --- Step 3: Configure Pins (Button - PB0) ---

    // Configure PB0 (Button) pin as 'Input with Pull-down'

    // First, clear the bits for PB0 (bits 0-3)
    GPIOB_CRL &= ~(0x0F << 0);

    // Set PB0 as 'Input with pull-up/pull-down'
    // MODE0[1:0] = 00 (Input)
    // CNF0[1:0]  = 10 (Input w/ pull-up/down)
    // Total: (0b1000 << 0) = 0x8
    GPIOB_CRL |= (0x08 << 0);

    // Select the PULL-DOWN resistor for PB0
    // Set the 0th bit of ODR to '0'
    GPIOB_ODR &= ~(1 << 0);

    // --- Step 4: Initialize Variables and Loop ---

    // LED pattern (uint8_t = 8-bit integer)
    // Lab sheet requires 3 LEDs to be lit at the same time
    // Initial state: LED1, LED2, LED3 (PA0, PA1, PA2) are ON
    uint8_t led_pattern = 0b00000111; // (PA2=1, PA1=1, PA0=1)

    // Direction state: 0 = Shift Right, 1 = Shift Left
    int direction = 0;

    // To catch the "press event", we store the last button state
    int last_button_state = 0;

    while (1) {

        // --- 4a. Button Check (Edge Detection) ---

        // Read the current state of the button
        int current_button_state = (GPIOB_IDR & (1 << 0)); // 0 or 1

        // If the button was NOT pressed before (0) AND is pressed NOW (1)...
        if (current_button_state == 1 && last_button_state == 0) {
            // Toggle the direction
            if (direction == 0) {
                direction = 1;
            } else {
                direction = 0;
            }
            // (or simply: direction = !direction;)
        }
        // Update the last state
        last_button_state = current_button_state;

        // --- 4b. Pattern Update ---

        if (direction == 0) {
            // SHIFT RIGHT (Rotate Right)
            // Take the LSB (Least Significant Bit) and move it to the MSB (Most Significant Bit)
            led_pattern = (led_pattern >> 1) | ((led_pattern & 1) << 7);
        }
        else {
            // SHIFT LEFT (Rotate Left)
            // Take the MSB (0x80) and move it to the LSB
            led_pattern = (led_pattern << 1) | ((led_pattern & 0x80) >> 7);
        }
        // --- 4c. Write to LEDs ---

        // Update only the first 8 bits (PA0-PA7) of GPIOA_ODR,
        // preserving the rest (higher bits).
        GPIOA_ODR = (GPIOA_ODR & 0xFFFFFF00) | led_pattern;
        // --- 4d. Delay ---

        delay_ms(100000);
    }
}
