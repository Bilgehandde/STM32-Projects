.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler

/* --- Hardware Address Definitions --- */
.equ RCC_BASE,      0x40021000            ; Reset and Clock Control (RCC) base address
.equ RCC_APB2ENR,   (RCC_BASE + 0x18)     ; APB2 Peripheral Clock Enable Register

.equ GPIOA_BASE,    0x40010800            ; GPIO Port A base address
.equ GPIOA_CRL,     (GPIOA_BASE + 0x00)     ; Port A Configuration Register Low (for Pins PA0-PA7)
.equ GPIOA_ODR,     (GPIOA_BASE + 0x0C)     ; Port A Output Data Register

Reset_Handler:
    ; 1. Initialize the Stack Pointer (SP)
    ldr sp, =_estack

    ; 2. Enable Clock for GPIOA
    ldr r0, =RCC_APB2ENR        ; Load address of RCC_APB2ENR
    ldr r1, [r0]                ; Read current value
    orr r1, r1, #(1 << 2)       ; Set bit 2 (IOPAEN - GPIOA Clock Enable)
    str r1, [r0]                ; Write the new value back

    ; 3. Configure Pins PA1, PA2, PA3, and PA4 as Outputs
    ldr r0, =GPIOA_CRL          ; Load address of GPIOA Control Register Low
    ldr r1, [r0]                ; Read current CRL value

    ; Create a mask to clear the config bits for PA1, PA2, PA3, PA4
    ; Each pin uses 4 bits (CNF+MODE). We need to clear bits 19 down to 4.
    ; Mask: 0b 0000 0000 0000 1111 1111 1111 0000 = 0x000FFFF0
    ldr r2, =0x000FFFF0         ; Load the 16-bit clear mask for PA1-PA4
    bic r1, r1, r2              ; Bitwise Clear (AND r1 with NOT r2)

    ; Create a mask to set the config bits for PA1-PA4
    ; We want 'General purpose output push-pull, max speed 2MHz'
    ; This corresponds to 0b0010 (CNF=00, MODE=10) for each pin.
    ; Mask: 0b 0000 0000 0010 0010 0010 0010 0000 = 0x00222220
    ldr r2, =0x00222220         ; Load the configuration mask for PA1-PA4
    orr r1, r1, r2              ; Bitwise OR to set the new modes
    str r1, [r0]                ; Write the new configuration back to CRL

    ; 4. Turn on LEDs connected to PA1, PA2, PA3, PA4
    ldr r0, =GPIOA_ODR          ; Load address of GPIOA Output Data Register
    ldr r1, [r0]                ; Read current output state (to preserve other pins)
    
    ; Create a mask to set bits 1, 2, 3, and 4 HIGH
    ; Mask: 0b 0001 1110 = 0x1E
    ldr r2, =0x1E               ; Load mask to set PA1, PA2, PA3, PA4
    orr r1, r1, r2              ; Set bits 1-4 HIGH, turning on the 4 LEDs
    str r1, [r0]                ; Write the new value back

loop:
    b loop                      ; Loop forever to keep LEDs on

.end
