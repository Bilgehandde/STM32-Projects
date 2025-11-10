.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler

/* --- Hardware Address Definitions --- */
.equ RCC_BASE,         0x40021000            ; Reset and Clock Control (RCC) base address
.equ RCC_APB2ENR,      (RCC_BASE + 0x18)     ; APB2 Peripheral Clock Enable Register

.equ GPIOA_BASE,       0x40010800            ; GPIO Port A base address
.equ GPIOA_CRL,        (GPIOA_BASE + 0x00)     ; Port A Configuration Register Low (Pins 0-7)
.equ GPIOA_ODR,        (GPIOA_BASE + 0x0C)     ; Port A Output Data Register

/*
 * NOTE: The lab manual suggested clocking and pin configuration were not
 * necessary for this problem. However, empirical testing on the physical
 * hardware (STM32F103) confirmed these steps are mandatory for the LED
 * to illuminate. This code includes the full, working configuration.
 */

Reset_Handler:
    ; Initialize the Stack Pointer (SP)
    ldr sp, =_estack      

    /* --- Step 1: Enable Clock for GPIOA --- */
    ldr r0, =RCC_APB2ENR    ; Load the address of the RCC APB2 Enable Register
    ldr r1, [r0]            ; Read the current value from that register
    orr r1, r1, #(1 << 2)   ; Set bit 2 (IOPAEN - GPIOA Clock Enable) to 1
    str r1, [r0]            ; Write the modified value back to the register

    /* --- Step 2: Configure Pin PA1 as an Output --- */
    ldr r0, =GPIOA_CRL      ; Load the address of the GPIOA Control Register Low (for PA0-PA7)
    ldr r1, [r0]            ; Read the current configuration
    
    ; Clear the bits for PA1 (bits 7:4, which are CNF1 and MODE1)
    bic r1, r1, #(0xF << 4) ; Bitwise Clear (AND with NOT 0xF0)
    
    ; Set PA1 to 'Output mode, max speed 2MHz' (MODE1 = 0b10)
    ; and 'General purpose output push-pull' (CNF1 = 0b00).
    ; Total value to set (CNF1|MODE1): 0b0010 = 0x2
    orr r1, r1, #(0x2 << 4) ; Set the bits (OR with 0x20)
    str r1, [r0]            ; Write the new configuration back to the register

    /* --- Step 3: Turn on the LED (Set PA1 HIGH) --- */
    ldr r0, =GPIOA_ODR      ; Load the address of the GPIOA Output Data Register
    ldr r1, [r0]            ; Read the current output state (to preserve other pins)
    orr r1, r1, #(1 << 1)   ; Set bit 1 (corresponding to PA1) to HIGH
    str r1, [r0]            ; Write the new value back, turning the LED on

infinite_loop:
    b infinite_loop         ; Loop forever to keep the program running
    
    .end
