# Cortex-M cookbook


WELCOME TO THE PLAYGROUND!

We are technic enthousiasts that loves to put our nose(r) into the lowest possible level of "bare-metall" programming on microcontrollers. 
If you also like beiing surrounded by electronic boards, LEDs that blinks, go full terminal (never go full terminal), push silicons to limits and so on, you are here at the right spot!

>[!INFO]
> The learn curve is a little bit disturbing at the beginning, but after a couple of days, you will surely feel yourself swimming like a fish between the flow of bits and transistors


This repository is a gathering of different small, "open-closed" projects that involves exclusively Cortex-M based microcontrollers.
We may use framework from vendors like STM's `HAL` or Raspberry Pi's `pico-sdk`, but the main goal is to understand, test and play uniquely and solely with Cortex-M features
Why? So that you may go from one Cortex-M silicon vendor to the other and still be confortable to develop, debug and optimize your code

One of the aim is to show how to set a development environment with only gnu-arm-none-eabi tool-chain. The only costs are laptop, Internet, electricity, the debuggers (depends) and the target boards.
What you will learn in this article:

Set up a complete Cortex-M development environment using only open-source tools
Manage and compile source code (internal and external) with CMake
Run a program with ThreadX
Flash and debug with
different debuggers (STLink, JLink, Raspberry Pi Debug Probe)
gdb from gnu-arm-none-eabi toolsuite
different gdbservers. How it works will be also explained.
On different Cortex-M architectures (targets: STM32H7, STM32F4, RP2040, RP2350)
We will see that for all situations (different hardware, different debugger, different gdbservers), only few things are to be modified! The development environment will stay the same, very simple and may be tailored as you need.

Currently we base ourselves on an pico 2 W board with an RP2350 microcontroller. It has indeed dual core, Cortex-M33 with security features like TrustZone and many more. It costs only aroudn 7-8$ and the debugger around 12$
You may still find some examples on 
