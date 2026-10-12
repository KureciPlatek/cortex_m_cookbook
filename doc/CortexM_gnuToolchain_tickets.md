# Introduction:

The idea is to show that we can fully control total Cortex-M development environment without any fully integrated black-boxed IDE

Is to show that IDE just do some steps to combine different mandatory elements (compiler, third party source etc... ) and actually brings nothing except more complexity, stronger coupling, and double installation of sources.

>[!SUMMARY]
>- Cortex-M used: M7, M4, dual M33, dual M0+, M55
>- GNU-ARM toolchain
>- VSCode, CMake
>- Debuggers: STLink, Black Magic Debug Probe, JLink, Raspberry Debug Probe

# Requirements & goals

## Requirements:

- Allow us and other to learn
- Target technologies that have use for industry: CRA, processing efficiency...
- Target things that we are thristy to learn as uC developper (ex: debug hardFault, busfault, RTOS efficiency and profiling, go disassembler, discover powerful debug methods in command-line gdb etc...)
- Let reader have the feeling that he did it himself (discover tech, understand it, etc...)
   
## Quality goals:

- Tutorials / projects takes max a couple of days to learn. If possible a couple of hours
- Open / closed principle: very simple tests, no big code or complexity, focus only on the topic
- No architecture overload, no AI overload, no agile/scrum overload, no Microsoft, no full IDE. Only technology understanding
- Idea of articles: explain the principle and the why, not the how. How to explain actually so that people could "feel" and carry on themselves -> Difficult to explain how to explain. But necessary. Strong review process

## Constraints:

- Cheap target for easy testbench install -> debugger, board, etc... (debugger is done). So inferior to 50 CHF investment and highly available
- Program showing tests/technologies are uC independent, they are only Cortex-M dependent. Vendor specific libraries like ST's HAL or Raspberry's pico-sdk may be used but code example should be as independent as possible

# Strategy

Proposed topics to learn:
- CRA technologies:
	- TrustZone
	- Secure Boot
	- Secure RTOS (ThreadX and/or FreeRTOS)
	- Here are engineers free to tests Cortex-M safety and security technologies
- Look at how the things we learn improves code efficiency? Means comparing
- Dual core, debug, MPI, some tips and tricks, RTOS/profiling
- Debug of HardFaults and others. Use fault handlers and debug with Core registers
- gdb hands-on: all the nice features, look at disassembler, go backwards. Be familiar with debug
- RP2350 is a good solution. But pico-sdk is not very nice to use
- Other, more powerful uC/Cortex-M proposes more features like SWO and tracing

## Rejected strategies:

- No certification here as it depends on a lot of things. And we are not TUV Sud
- No IDE. Goal here is to open the black-box and understand. IDE does the opposite. You may integrate it if it pleases you

***
## SYSTEM ISSUES

**New issues:**  

| Issue | Description                                                                                                                                                                                                 | state |
| ----- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----- |
| SYS_1 | ==Process== Create a branch for each DevEnv or couple of DevEnv. Avoid a single, long linear Git line                                                                                                       | NEW   |
| DOC_2 | ==ARM-Cortex== Explain and demystify CMSIS and other CMSIS (like DSP) files                                                                                                                                 | NEW   |
| DOC_3 | ==Debug== Explain/demystify/depict the different debug interfaces, _DAP_, _TAP_, _JTAG_, _SWD_ etc..., their role, their physical interfaces, connectors (_SWD + UART_, 20 pins connectors etc..) and so on | NEW   |
| DOC_4 | ==Linker== Understand linker files and how they work together with reset_handlers and startup.s files                                                                                                       | NEW   |
| SYS_4 | ==Blog== test gdbgui: [gdbgui](https://www.gdbgui.com/)                                                                                                                                                     | NEW   |
| SYS_7 | ==debug== Use of `gdb tui`                                                                                                                                                                                  | NEW   |
|       |                                                                                                                                                                                                             |       |


**Work In Progress issues:**  

| Issue | Description                                                                        | state |
| ----- | ---------------------------------------------------------------------------------- | ----- |
|       |                                                                                    |       |


**Resolved issues:**

| Issue | Description                                                                                                                                                                   | state           |
| ----- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------- |
| SYS_2 | ==CMake== Restructure all CMake files for them to be better structured and more clean<br>- Improvement: use target_sources() to avoid specifing those at add_executable() cmd | DONE - #0aa5813 |
| DOC_1 | ==Blog== Create all documentation and explain the full work and process on each DevEnv                                                                                        | DONE            |
| DOC_7 | ==Blog== Fill up Debuggers.md file with important data or remove it                                                                                                           | DONE            |
|       |                                                                                                                                                                               |                 |


**Rejected issues**

| Issue | Description                                                                                                                                | state                        |
| ----- | ------------------------------------------------------------------------------------------------------------------------------------------ | ---------------------------- |
| SYS_5 | ==Devenv== Look if **CMakePresets.json** are good to configure the projects. If only for a family of uC or only a vendor, maybe not use it | DROP - not relevant          |
| SYS_3 | ==Blog== Create a project with an NXP microcontroller                                                                                      | DROP - work on Cortex-M only |
|       |                                                                                                                                            |                              |

***
## SOFTWARE ISSUES

**New issues:**  

| Issue  | Description                                                                                                                                                                                            | state |
| ------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----- |
| SFW_1  | ==C/C++ libraries== Check about the different libraries that are available/used/recommended by GNU-ARM toolsuite: _picolib_, _newlib_, _nanolib_, _stdlib_ etc...                                      | NEW   |
| SFW_3  | ==gdb== Look at some debug cases with following commands:<br> - `(gdb) layout asm`<br> - `(gdb) dprintf <br> <br> - `(gdb) bt <num> / up /down / frame (f) idx`<br> - `(gdb) tui`                      | NEW   |
| SFW_13 | ==gdb== look at how to optimize use of `.gdbinit` (Black Magic Book page 29)                                                                                                                           | NEW   |
| SFW_14 | ==dual core== Set a dual core debug session on Cortex-M33 or cortex-M0, put a message passing interface and RTOS. Two tasks exchange data, each runs on a different core. Stop core_1, core_0 or both. | NEW   |
| SFW_15 | ==debug== Set problem with pointer near an uint16_t, cast the uint16 -> set to 0, pointer is then fucked up. How to debug with `HardFault()` or any handler                                            | NEW   |
| SFW_2  | ==print()== Cleanup and organize selection between SWO or UART and maybe RTT                                                                                                                           | NEW   |
| SFW_16 | ==ITM== Benchmark ITM vs RTT and optimize its code                                                                                                                                                     | NEW   |


**Work In Progress issues:**  

| Issue | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         | state                                                               |
| ----- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- |
| SFW_4 | ==DevEnv7== RP2350, ThreadX (Eclipseless), Raspberry Debug Probe<br>Look at official ThreadX git repo, in ports/cm33/example/build/ and look at secure and non secure mode, how TZ is implemented and the function callback story (it seems that some callback have to be implemented when a Cortex-M has a TrustZone, recommended by ARM).<br>Look at how to debug a HardFault and LR corruption problem<br>Seems that RP2350 always boot in Secure code, maybe is NSC and Secure ThreadX required | WIP - #1b0f7a7<br>Has a core HardFault IRQ when executing scheduler |
|       |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |                                                                     |


**Resolved issues:**

| Issue  | Description                                                                                                                                                                                           | state           |
| ------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------- |
| SFW_8  | ==DevEnv5== STM32F4, ThreadX (Eclipseless, from official Github repo direct), BlackMagic debug probe, STMCubeless (keep only required specific files)                                                 | DONE            |
| SFW_11 | ==DevEnv8== RP2350, FreeRTOS, Raspberry Debug Probe, MQTT<br>Warning about memory required for lwIP                                                                                                   | DONE - #86f3157 |
| SFW_5  | ==DevEnv2== With RP2040, ThreadX (Eclipseless), Raspberry Debug Probe, Github/common Pico-SDK                                                                                                         | DONE #d53d3c3   |
| SFW_10 | ==External Sources== restructure management of external source from GitHub repositories.<br>Get the official Azure_ThreadX repo instead of the one provideed by STMicroelectronics (x-cube-azrtos-h7) | DONE - #0aa5813 |
| SFW_12 | ==DevEnv7== Restructure threads and mqtt connection management. One task to manage MQTT connection, and transfers, one task to read temperature, one task to read gyroscopes                          | DONE - #20746fe |
| SFW_6  | ==DevEnv3== RP2040, ThreadX (Eclipseless) BlackMagic debug probe, Github/common Pico-SDK                                                                                                              | DONE            |
| SFW_9  | ==DevEnv6== STM32H7, ThreadX (Eclipseless), BlackMagic debug probe, STMCubeless (keep only required specific files)                                                                                   | DONE            |
| SFW_12 | ==Tracing== Look how to integrate it, `ITM_SendChar(*msg++)` for example and other macros from CMSIS                                                                                                  | DONE            |

**Rejected issues**

| Issue | Description                                                                      | state                 |
| ----- | -------------------------------------------------------------------------------- | --------------------- |
| SFW_7 | ==DevEnv4== STM32F4, ThreadX (Eclipse plugin), STLink debug probe, STMCubeIDE/MX | REJECTED - No STMCube |
|       |                                                                                  |                       |
