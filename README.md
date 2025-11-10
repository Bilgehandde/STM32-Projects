# STM32 Embedded Systems Portfolio

This repository contains my portfolio of embedded systems projects for the STM32F103 microcontroller, based on the ELEC 335 (Digital Systems) lab curriculum at Gebze Technical University.

This collection demonstrates foundational skills in both **ARM Assembly** and **Register-Level C programming**.

## Projects

Each project folder contains its own detailed `README.md` file explaining the objective, implementation, and key concepts demonstrated.

### [Lab-1: Assembly Basics](./Lab-1/)
*A collection of standalone ARM Assembly programs written for the Cortex-M3 processor, demonstrating direct memory-mapped register access.*

* **[problem2: Light 1 LED](./Lab-1/problem2/)**
    * The "Hello, World!" of hardware. Sets the GPIOA clock, configures pin PA1 as an output, and writes to the `ODR` to light a single LED.
* **[problem3: Light 4 LEDs](./Lab-1/problem3/)**
    * Demonstrates multi-pin control by using bitmasks to configure and light 4 LEDs (PA1-PA4) simultaneously.
* **[problem4: Assembly Delay Routine](./Lab-1/problem4/)**
    * A complete program with a stack and vector table. Implements a `bl`/`bx lr` subroutine to create a blocking delay, used to toggle an LED at ~1Hz.

### [Lab-2: C-Language (Register-Level)](./Lab-2/)
*A series of C-language projects that control hardware by writing directly to register addresses, without using the HAL or LL libraries.*

* **[problem1: C LED Toggle](./Lab-2/problem1/)**
    * Configures the RCC clock and GPIOC registers (PC13) to toggle the on-board LED using a C-based busy-wait delay loop.
* **[problem2: C Button Input](./Lab-2/problem2/)**
    * Configures a GPIO pin (PA0) as an input with an internal pull-down resistor. Polls the `IDR` (Input Data Register) to control the LED state.
* **[problem4-5: Knight Rider Animation](./Lab-2/problem4-5/)**
    * The capstone project. Implements the "GiTT" (Knight Rider) animation on 8 LEDs. Includes logic for button-controlled direction (Problem 4) and an automatic "bounce" pattern (Problem 5).
