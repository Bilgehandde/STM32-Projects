.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler

/* --- Hardware Address Definitions --- */
.equ RCC_BASE,         0x40021000
.equ RCC_APB2ENR,      (RCC_BASE + 0x18)

.equ GPIOA_BASE,       0x40010800
.equ GPIOA_CRL,        (GPIOA_BASE + 0x00)     ; Port A Config Register Low (PA0-PA7)
.equ GPIOA_ODR,        (GPIOA_BASE + 0x0C)     ; Port A Output Data Register

/* --- Constants --- */
.equ GPIOA_CLOCK_ENABLE_MASK, (1 << 2)     ; Mask for IOPAEN (bit 2)
.equ PA1_CLEAR_MASK,          (0xF << 4)   ; Mask to clear PA1 config bits (CNF1, MODE1)
.equ PA1_CONFIG_OUTPUT,       (0x2 << 4)   ; Mask to set PA1 as Output Push-Pull, 2MHz
.equ PA1_BIT_MASK,            (1 << 1)     ; Mask for PA1 in ODR (bit 1)
.equ DELAY_COUNT,             2000000      ; Loop count for ~0.5 sec delay at 16MHz

/* --- Vector Table (Minimal) --- */
; This table tells the processor where the Stack is and where to go on Reset.
.section .vectors
vector_table:
    .word _estack           ; Initial Stack Pointer (Top of Stack)
    .word Reset_Handler     ; Reset Vector (Code entry point)

/* --- Reset Handler (Main Program) --- */
.section .text
Reset_Handler:
    ldr sp, =_estack      ; Initialize Stack Pointer

    /* --- Step 1: Enable GPIOA Clock --- */
    ldr r0, =RCC_APB2ENR
    ldr r1, [r0]
    orr r1, r1, #GPIOA_CLOCK_ENABLE_MASK
    str r1, [r0]

    /* --- Step 2: Configure PA1 as Output --- */
    ldr r0, =GPIOA_CRL
    ldr r1, [r0]
    bic r1, r1, #PA1_CLEAR_MASK  ; Clear current PA1 config
    orr r1, r1, #PA1_CONFIG_OUTPUT ; Set PA1 as output
    str r1, [r0]

main_loop:
    /* --- Step 3a: Turn LED ON (Set PA1 HIGH) --- */
    ldr r0, =GPIOA_ODR
    ldr r1, [r0]
    orr r1, r1, #PA1_BIT_MASK   ; Set bit 1 HIGH
    str r1, [r0]

    /* --- Step 3b: Call Delay Subroutine --- */
    bl Delay_Half_Second    ; 'Branch with Link' (call) the delay function

    /* --- Step 3c: Turn LED OFF (Clear PA1 LOW) --- */
    ldr r0, =GPIOA_ODR
    ldr r1, [r0]
    bic r1, r1, #PA1_BIT_MASK   ; 'Bit Clear' (sets bit 1 to 0)
    str r1, [r0]

    /* --- Step 3d: Call Delay Subroutine --- */
    bl Delay_Half_Second    ; Call the same delay again

    /* --- Step 3e: Repeat --- */
    b main_loop             ; Unconditional branch back to the main loop

/* --- Subroutine: Delay_Half_Second --- */
; A simple delay loop function.
; Wastes CPU cycles for approximately 0.5 seconds.
; Input: None
; Output: None
; Clobbers (uses): R3
Delay_Half_Second:
    ldr r3, =DELAY_COUNT    ; Load the loop counter value into R3
delay_inner_loop:
    subs r3, r3, #1         ; Subtract 1 from R3 (and update status flags)
    bne delay_inner_loop    ; Branch (if Not Equal to zero) to loop start
    bx lr                   ; 'Branch and Exchange' to Link Register (Return)

    .end

/* --- Stack Definition --- */
; Define a memory area for the stack
.equ Stack_Size, 1024       ; Define stack size (1KB)
.section .stack             ; Place this in a '.stack' section
.align 3                    ; Ensure 8-byte alignment
stack_bottom:
.space Stack_Size           ; Reserve the stack space
_estack:                    ; Define the symbol '_estack' (top of stack)
