# Introduction


Have you ever be amazed at how IDE can be heavy?
Have you enough to have one IDE per project, each and single one of them weighing sometimes tenths of GB?
Hadn't you ever wanted to have a light, yet powerful development and debug environment that allows you to use programs you prefer?


Well more generally...

**Do we need IDE to develop and debug on Cortex-M based hardware?**

![[diagrams/grumpyCat.jpg]]

No, you don't need all those over-heavy IDEs that sometimes don't even work! 

Cortex-M offers powerful debug environment, which is the same for all Cortex-M based microcontrollers and the GNU-ARM toolsuite is all you need.

Then, which tool/equipment you need/want is up to you.

### Why you are here

>[!Abstract] 
>The aim of this article ist to show how to set a development environment with only **gnu-arm-none-eabi** tool-chain. The **only costs** are **laptop, Internet, electricity**, the **debug probes** (depends) and the **target boards**.
>What you will learn in this article:
>- Set up a **complete Cortex-M development environment** using only open-source tools
>- Manage and compile source code (internal and external) with **CMake**
>- Run a program with **ThreadX**
>- **Flash** and **debug** with
>	- **different debug probes**  (STLink, JLink, Raspberry Pi Debug Probe)
>	- **gdb** from **gnu-arm-none-eabi** toolsuite
>	- **different gdbservers**. How it works will be also explained.
>	- On **different Cortex-M architectures** (targets: STM32H7, STM32F4, RP2040, RP2350)
> 
> We will see that for all situations (different hardware, different debug probe, different gdbservers), only a few things are to be modified! The development environment will stay the same, very simple and may be tailored as you need. 

### Required equipment: 

For this tutorial, you will need:
- An STM32H723ZG nucleo board [NUCLEO-H723ZG](https://www.st.com/en/evaluation-tools/nucleo-h723zg.html)
- STM32CubeH7 HAL [STMicroelectronics/STM32CubeH7 GitHub](https://github.com/STMicroelectronics/STM32CubeH7)  installed wherever you want
- ThreadX Github repository [Eclipse-threadx/threadx GitHub](https://github.com/eclipse-threadx/threadx) Installed wherever you want
- GNU-ARM toolchain [Arm GNU Toolchain Downloads – Arm Developer](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) 
- Linux (works also on Windows but explained later)
- Installed openOCD with aptitude for Linux


# 1. Theory about debugging microcontrollers with `gdb`

Nothing new here, `gdb` stands for **G**NU **D**e**B**ugger. It is able to debug programs coming from many different programming languages, like C, C++ mainly, but also Fortran, Go and others. It knows how the programming language standard works, and provides an environment in which the program runs, and all information from inside the program, as well as execute it step by step, print registers and memory, value of variables, call specific functions and so on.


But **gdb** is only the debugger, it knows only to stop, run, watch, breakpoints and so on, on C/C++/ASM (and others) languages. On a machine like the laptop, it is easy because the OS is providing a full space for `gdb` to run the program in the language environment (with different sections, stack pointer and so on for people a little bit familiar with C/C++).

However, on microcontroller, hardware it is almost each time different and how to set breakpoints, look at variable, look at memory is for it unknown. There is no OS providing the environment.


That's when **gdbserver** comes in. It does this translation. `gdb` will connect to this server to start a debug session and its basic actions, like setting breakpoints, look at memory and so on will be translated by debug probe, onto the target. Actually also flashing, erasing and other mandatory/useful features when working on a microcontroller are provided by this `gdbserver`

This gdbserver has to know what kind of architecture we are dealing with, used memory technology, clock frequency of debug probe (STLINK, JLink...), configure its server and so on. Sometimes it is automatic, sometimes you have to configure yourself (it is the case of OpenOCD for example)


There is the last part, `gdbserver` requires a hardware part, the famous debug probe, to execute all the actions (breakpoint, read/write memory, run functions...) onto the target/board, through JTAG, SWD or other interface/protocol.


**Overview of debug on external target**:
![[diagrams/gdb_gdbServer_view.svg]]

**With:**

| Element                  | Description                                                                                                                                                                                                                                                                                                                                                                                                                |
| ------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`gdb`**                | The **"master"**. Runs on computer and acts as a client.<br>We will load, flash and debug our program on target with it. **It is our user interface**.                                                                                                                                                                                                                                                                     |
| **`gdbserver`**          | The **"translator"**. Also runs on computer.<br>It finds the debug probe, communicates with it, knows what kind of target is on the other side of the `SWD/JTAG` interface. Its main job is to "translate" commands coming from gdb to commands which are understandable by the target. It knows how to set breakpoint, read registers and memory and so on. It provides a TCP socket for `gdb` to connect to.             |
| **`debug probe`**        | The **hardware**, which translates target's debug communication to another communication understandable by computer (mostly USB) and is used by `gdbserver` to manipulate target.                                                                                                                                                                                                                                          |
| **`target`**             | Cortex-M on which we run and debug our application program                                                                                                                                                                                                                                                                                                                                                                 |
| **`UART/Serial`**        | _Optional_. Some debug probes (even small ones) offers this very handy virtual serial interface. It is useful because one could forward `printf()` from program (on target), to this `UART` interface. Then debug probe will forward this `UART` to a  `/dev/` (UNIX) or `COM` port interface (Windows) and tadaaaa, you see all your `printfs()` live, while debugging.<br>It will be mentioned as "UART" in this article |
| **`SWD / JTAG / Trace`** | The debug interface of our Cortex-M target. All Cortex-M targets implements this interface, it is part of CoreSight technology from ARM. It is also possible to use `JTAG`, which is more democratized in microcontroller world, but it requires one wire more than `SWD`<br>`Trace` is for code coverage and is not handled by every debug probe / debug session                                                          |


>[!WARNING]
>Everything is very simply said, but this diagram could be in reality a little bit more complex. Bigger debug probes and architectures requires another level.

>[!TIP]
>Now that we understand that `gdbserver` and `debug probes` are separate components, it is important to recognize that compatibility depends on the `gdbserver` implementation. Some `gdbservers` support specific hardware debuggers, while others do not. Consequently, certain `gdbservers` are better suited to particular hardware than others. 
>
>For example STLINK-V3E works better with the official `STLinkServer` than the open-source project `stlink-org` ([GitHub - stlink-org/stlink: Open source STM32 MCU programming toolset](https://github.com/stlink-org/stlink)) 

## Put it all together

In order to run a debug session on a Cortex-M target, you have a couple of things to run:

|     Element     | Description                                                                                                                                                                                                 | Example                                     |
| :-------------: | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------- |
| **`debug probe`**  | with interface compatible with Cortex-M's proposed interface (SWD, JTAG, etc...)                                                                                                                            | STLink, JLink...                            |
| **`gdbserver`** | in order to set debug probe available to `gdb` and translate `gdb` commands into target-specific commands. Some debug probes like `JLink` already embeds it all together and you just have to start debug session | `STLinkServer`, `OpenOCD`, `JLinkGDBServer` |
|    **`gdb`**    | which will debug, flash and/or manipulate the program onto the target                                                                                                                                       | `arm-none-eabi-gdb`, `gdb-multiarch`        |

### Typical use of `gdb` with a `gdbserver`:

Here are the steps to set-up the debug and development environment:

1. Plug and power your target, connect the debug probe hardware to it
2. Open one terminal to run `gdbserver`, it will prompt that it waits for connections
3. Open another terminal, go where your compiled `.elf` file is and start `gdb`, providing this `.elf` file to it
4. In `gdb` prompt: 
	- attach to target
	- load/flash the bare-metal `<program>.elf` file
	- Set your breakpoints, and other commands before launch
	- run program and debug it


**Diagram representing all steps:**
![[diagrams/gdb_gdbserver_example_project_overview.svg]]

- Terminal 1: where `gdbserver` runs
- Terminal 2: where you will run the debug session with `gdb`
- Terminal 3 (option): all `printf()` will be printed here (explained later)

>[!INFO]
>Terminal 1 with `gdbserver` is actually just print logs, you can't send any commands with it or something else. It only prints logs about what is happening on target. You may run it in background if you want. Personally I keep it on so I can see if there is any problem with target, or if for example I am trying to access an unauthorized memory area.

# 2. Put into practice: STM32H723ZG first project

Now we will look at how to start a debug session on an STM32H723ZG Nucleo board. This board has the advantage to embed a debug probe, the `STLink-V3` directly on its board.

To avoid any development, you may download project directly from our github repository:
[github link here]

**Content:**
We will execute the different steps described earlier in this tutorial:
1. Set-up hardware
2. Run `gdbserver`
3. Run `gdb`
4. (optional) configure `printf()` to see logs

>[!INFO] Prerequisite: 
>- You have installed all required programs mentioned earlier
>- You have configured variable `$PRJ_EXTSRC_PATH` in your environment for CMake
>- You have  successfully compiled a `prj_h7.elf` from project `prj_h7/` (project for stm32h723zgtx) with **`-g3` debug flag** (maximal debug level = have symbols in your .elf file)


**Optional:**
For those who thinks a GUI is sexier, it is possible to configure VSCode to use the development and debugging environment. But in this case it would be easier to directly install the different development environment plugins.

 
>[!IMPORTANT]
>All of the steps described here were done on Linux, but it almost be the same on Windows. Except for `/dev/`, which are `COM Port` instead 

## 1. Set-up hardware 

### Nucleo board setup

We will do our first test on a Nucleo board, here is how its integrated debug probe is set and configured:
![[diagrams/STM32H723ZG_overview_debugger.jpg]]

**Where:**

| Square | Description                                                                                                                                                                                                                                          | Connector / Reference | Action                                                    |
| :----: | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :-------------------: | :-------------------------------------------------------- |
| Orange | We connect the USB cable to the `ST-Link V3` debug probe                                                                                                                                                                                                |          CN1          | Connect USB to it                                         |
|  Red   | The debug probe, an STM32F723, it is actually the `ST-Link V3` itself. This chip connects to the SWD of our target (the STM32H723ZG below)                                                                                                              |          U7           | Keep JP1 (over it) open. Otherwise it deactivates ST-Link |
| Yellow | An auxiliary, standardized debug interface, it is in case you want to use another debug probe (you will have to deactivate `ST-Link` though). You may have heard its name: `MIPI10` (for 10 pins). For example `JLink` and `Lauterbach` connects there. |          CN5          | Leave                                                     |
| Green  | power supply of our target. Keep it to "`ST-Link V3`"                                                                                                                                                                                                |          JP2          | Keep as shown                                             |

Here is how things are connected inside the Nucleo board:

![[diagrams/stm32h7_setup.svg]]


| Element     | Description                                                                                                                                                                                                                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `UART3`     | As it is hard-wired on Nucleo board, from STM32H7 to `STLink`, we use the `UART3` instance with pins `GPIO_D8` and `GPIO_D9`. It is possible to modify it, but soldering has to be involved. This `UART` is important as it provides all `printf()` through USB to our development environment! |
| `LED green` | Like `UART3`, hard-wired on Nucleo board on `GPIO_B0`                                                                                                                                                                                                                                           |
| `LED red`   | Like `UART3`, hard-wired on Nucleo board on `GPIO_B14`                                                                                                                                                                                                                                          |
| SWD         | Cortex-M CoreSight's interface                                                                                                                                                                                                                                                                  |
| Clock       | On STM32H723ZG Nucleo board, external high-speed clock (HSE) of the micronctroller is actually not a crystal clock, but a clock signal provided by `ST-Link -V3`. Which means if you deactivate `ST-Link`, you will have to set another clock source on STM32H7!                                |

For deeper details, look at document: [STM32H7 Nucleo-144 boards (MB1364) - User manual](https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf)
Or in source code of `prj_h7`.

## 2. Set-up and run `gdbserver`

### Check presence of device

Plug your STM32H723ZG Nucleo or discovery board to your development laptop where you will run the debugging session.

Then take a quick look if the following device appears (on Linux):
```bash
$ ls /dev/
...
stlinkv3_
stlinkv3_0
stlinkv3_1
...
serial/by-id/ -> ../../ttyACM0
serial/by-path/ ->  ../../ttyACM0
...
```

`/dev/serial/by-path/` and `/dev/serial/by-id/` are just link to `/dev/ttyACM0` which is the virtual serial communication to retrieve `printf()`'s sent logs from target.

>[!INFO]
>For Windows, go into Device Manager and check that new COM Port in COM & LPT, is listed.

>[!IMPORTANT] Working with WSL
>It is possible to work with WSL, but you have to configure a couple of more things:
>1. Forward USB to wsl with usbipd: [Connect USB devices | Microsoft Learn](https://learn.microsoft.com/en-us/windows/wsl/connect-usb)
>2. For openOCD, you may have to configure udev rules to allow openOCD to access to usb connections. You will find all explanations here, section udev: [WSL support · dorssel/usbipd-win Wiki · GitHub](https://github.com/dorssel/usbipd-win/wiki/WSL-support)

### Configure `openOCD` scripts


When using `openOCD`, good old `tcl` scripts are used to configure targets (which microcontroller) and interface (which communication between debug probe and microcontroller). In Linux, those scripts are to be found in:

```bash
$ ll /usr/share/openocd/scripts/
total 188
-rw-r--r-- 1 root    root     1659 Jul 16  2025 bitsbytes.tcl
drwxr-xr-x 6 root    root    65536 Aug  4  2025 board
drwxr-xr-x 5 root    root     4096 Nov 19  2024 chip
drwxr-xr-x 2 root    root     4096 Aug  4  2025 cpld
drwxr-xr-x 4 root    root     4096 Nov 19  2024 cpu
-rw-r--r-- 1 root    root      956 Jul 16  2025 file_renaming.cfg
drwxr-xr-x 2 root    root     4096 Aug  4  2025 fpga
-rw-r--r-- 1 jeremie jeremie   510 Apr  9  2025 gdb_helper.tcl
drwxr-xr-x 5 root    root    12288 Aug  4  2025 interface
-rw-r--r-- 1 root    root     1021 Jul 16  2025 mem_helper.tcl
-rw-r--r-- 1 root    root     4423 Jul 16  2025 memory.tcl
-rw-r--r-- 1 root    root     2121 Jul 16  2025 mmr_helpers.tcl
drwxr-xr-x 6 root    root    36864 Aug  4 07:57 target
drwxr-xr-x 2 root    root     4096 Aug  4  2025 test
drwxr-xr-x 2 root    root     4096 Aug  4  2025 tool
```

Here, two folders are important: `target/` which lists all supported targets and `interface/` which lists all supported interfaces.

If you go to `openOCD` website, you will learn from its creator that it is an open-source software (therefore "open"OCD). But you will also learn that those scripts may not work, as vendors may have modified things. `openOCD` itself will recommend you to use vendor's supplied scripts as the default that you got with `apt install openocd`.

If you used STM32CubeIDE, you can indeed confirm it, as if you search in `plugins/` folder of STM32CubeIDE install, you will find all those `openOCD` scripts again! For me, on my Windows I have:
```bash
/ST/STM32CubeIDE_1.18.1/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.debug.openocd_2.3.100.202501240831/resources/openocd/st_scripts$ ll
total 4
drwxrwxrwx 1 jeremie jeremie 4096 Jul  2  2025 ./
drwxrwxrwx 1 jeremie jeremie 4096 Jul  2  2025 ../
drwxrwxrwx 1 jeremie jeremie 4096 Jul  2  2025 board/
-rwxrwxrwx 1 jeremie jeremie  510 Apr  9  2025 gdb_helper.tcl*
drwxrwxrwx 1 jeremie jeremie 4096 Jul  2  2025 interface/
-rwxrwxrwx 1 jeremie jeremie  976 Apr  9  2025 mem_helper.tcl*
drwxrwxrwx 1 jeremie jeremie 4096 Sep 17 11:45 target/
```

>[!IMPORTANT]
>As said, the default scripts may not work, so we have to use the official scripts of ST Microelectronics.

>[!INFO]
>Luckily, you don't have to download and install STM32CubeIDE to get all those scripts, ST Microelectronics kindly provides them all on Github: [STMicroelectronics/OpenOCD GitHub](https://github.com/STMicroelectronics/OpenOCD)  In section `tcl/` you will find all official `openOCD` scripts for ST Microelectronics targets.

To do so, we copy all those scripts to openOCD's default search path. In other words, we add them here: `/usr/share/openocd/scripts`. You may either replace default STM32 targets and defaults interface scripts. Or you may add `st_` prefix to STM's `target/` and `interface/` folder names and add them near the old, default folders.

```bash
ll /usr/share/openocd/scripts/
total 188
...
# Default scripts
drwxr-xr-x 6 root    root    65536 Aug  4  2025 board
drwxr-xr-x 5 root    root    12288 Aug  4  2025 interface
drwxr-xr-x 6 root    root    36864 Aug  4 07:57 target
...
# STM32 official scripts
drwxr-xr-x 2 root    root     4096 Jul 11  2025 st_board
drwxr-xr-x 2 root    root     4096 Jul 11  2025 st_interface
drwxr-xr-x 2 root    root     4096 Aug  4 10:48 st_target
...
```

I modified the `openOCD` tcl configuration scripts so that they fetch in those `st_` prefixed folders instead of the default one. If you look in `stm32h7_openOCDsrv_stlink.cfg` file, you will see:

```tcl
# Debug of an STM32H723ZGTx chip
source [find st_interface/stlink-dap.cfg]
...
source [find st_target/stm32h7_git.cfg]
```

### Run `openOCD` as  `gdbserver`

Now that `openOCD` is installed and configured, we can run it.

There are many different programs that fulfill the role of a `gdbserver`. One which is open-source and massively used with GNU-ARM toolchain is `openOCD`. It supports many hardware and is quite easy and simple to run.

When all previous steps are good, just call the config file from prj_h7 to start `openOCD` as a `gdbserver`. Configuration file prj_h7 is available in our github project examples.

Reminder, `TERMINAL x` are the ones described earlier in section [[#Typical use of `gdb` with a `gdbserver`]].

**TERMINAL 1**: run `gdbserver` (`openOCD`)

```bash
# Run openOCD for stm32h7
~/.../prj_h7 $ openocd -f /path/to/prj_h7/scripts/stm32h7_openOCDsrv_stlink.cfg

pen On-Chip Debugger 0.12.0+dev-snapshot (2025-07-16-14:15)
Licensed under GNU GPL v2
...
stm32h7x_cti_prepare_restart_one
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : STLINK V3J6M2 (API v3) VID:PID 0483:374E
...
Info : [STM32H723ZGTx.cm7] starting gdb server on 3333
Info : Listening on port 3333 for gdb connections
Info : [STM32H723ZGTx.cm7] external reset detected
# Here it is waiting for connections coming from gdb on local, port 3333

```

As said earlier, you may also let this `gdbserver` run in background if logs doesn't interest you.

## 3. Run `gdb`

In **TERMINAL 2**: we run the debugger. For that, you may provide the elf file you want to debug as follow:

```bash
$ arm-none-eabi-gdb path/to/your/prj_h7.elf
```

Or, just run `arm-none-eabi-gdb` and then, in prompt, fetch the .elf file with:
```bash
(gdb) file path/to/file/binary_to_debug.elf
```

Then, some steps are required:
1. Connect to `gdbserver` with `target extended-remote <port>`
2. load binary to target (flash it)
3. Set some configuration and breakpoints
4. run program

#### 1. Connect to `gdbserver`

Just be careful to provide the exact port provided by `gdbserver` after you started it (prompted in `gdbserver`'s output). In our case it is 3333:

```bash
(gdb) target extended-remote :3333
Remote debugging using :3333
```

>[!NOTE]
>`<port>` is actually configured in `<debug_session_name>.cfg` file, line: `set GDB_PORT 3333`. You may change it for one you prefer.

When connected to the `gdbserver`, back to Terminal 1 with `gdbserver` following output should be prompted:

```bash
...
Info : [STM32H723ZGTx.cm7] external reset detected
Info : accepting 'gdb' connection on tcp/3333
[STM32H723ZGTx.cm7] halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x0801eb7c msp: 0x24050000
...
Info : New GDB Connection: 1, Target STM32H723ZGTx.cm7, state: halted

```

>[!NOTE]
>`gdb` may be called prior to `openOCD`. But if you try to connect to it with command 
>`target extended-remote :3333`, it will not work, as `gdbserver` is not yet available 

#### 2. Flash program
Then you may start loading and have fun on terminal 2 (`gdb`):
```bash
(gdb) load # Flash program
Loading section .isr_vector, size 0x2cc lma 0x8000000
...
Loading section .data, size 0xcc lma 0x801ecd8
Start address 0x0801ea3c, load size 126368
Transfer rate: 18 KB/sec, 936 bytes/write.
```

#### 3. Set some configuration

From this point on, you may analyze a little bit available symbols, memory and so on. An army of commands from gdb is here at your service to prepare the debug session. Like setting prints as pretty, set breakpoints, call some functions separately and so on.

>[!IMPORTANT]
>All steps mentioned earlier look fastidious to execute each time. For that have gdb developers done something rather useful: configuration of an initialization file.
>It is very simple and all you have to do is copy the commands you would run on `(gdb)` prompt in it. Exactly the same way.
>
>Then run gdb, giving as argument the configuration file:
>```bash
>$ gdb -x scripts/gdbinit_stlink.gdb
>```
>You may add any command you want in it, like `set print pretty`, `break myFunc()` etc...

#### 4. Run program

When ready, start program on target, invoking `run` or `r`:

```bash
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: ~/work/prj_h7/bin/prj_h7.elf
```

## 4. (optional) Configure `printf()`

In a C program, functions from standard library, like `malloc()`, `printf()`, `fopen()` and so on, requires some very low functions, like `_write`, `_sbrk`, `_close`, `_read` ,to be implemented. Those functions are generally implemented by the OS itself.

But on a "bare-metal" program, there is no OS, so those functions are not implemented.

For this reason, in CMSIS, `syscalls.c` and `sysmem.c` exists. They implement a _VERY_ basic version of those functions. Or just are stubs, required for compilation to work. `syscall.c` stands for system call and `sysmem.c`, for system memory. The programmer is free to implement or customize these functions according to the application’s requirements and the target system.


>[!NOTE]
>`syscall` and `sysmem` functions are all declared with `__attribute__((weak))` for user to be able to redefine it somewhere in its own code, without having an "already declared" conflict form compiler 

#### Code modification to redirect `printf()`

When looking closer, `printf()` actually requires `_write()` function. This function is defined `syscall.c`:

```C
__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    // Here, __io_putchar() could be defined somewhere in our program, to send the character through UART. Therefore, when calling printf(), we will redirect stream of character to a defined UART communication.
    __io_putchar(*ptr++);
  }
  return len;
}
```

All we have to do is to modify `__io_putchar()` in our program for it to call `UART_send()`.

Doing so, each time `printf()` is called, it calls at the end `UART_Transmit(char*)`, which prints all characters one after the other to the `UART` communication.

For that, declare `__io_putchar(int ch)` somewhere in your code and implement it as follow:
```C
int __io_putchar(int ch)
{
   HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
   return ch;
}
```

>[!INFO]
>On Nucleo board, it is UART 3, which is hardwired to `ST-Link V3`. So it has to be this one given to `HAL_UART_Transmit()`.
>You may select another one but you have to solder wires out.

#### Consume `UART`/`printf()` messages

All messages should be now available on the serial interface. This interface is provided by `ST-Link V3` as a virtual serial interface. On Linux you can find it in the `/dev/` files:

```bash
$ ls /dev/tty*
/dev/ttyACM0
```

You may select the program you want to open and stream this serial device on Linux. I used `minicom` which is fine.

Now when you run the program, you should also see log messages coming on your serial reader


#### Conclusion about `printf()` over `UART`

This way to become logs is quite handy and quite used in the industry. Many Debug probe proposes for this reason, an UART interface. But it comes on my opinion with a disadvantage: you lose one UART interface. Another disadvantage is actually dangerous: if you let printf() active in your release, a hacker could look at information about what is happening inside your program with a simple 5$ worth serial to USB cable!

There are two better options:
- Either use RTT, which uses internal ring buffer in SRAM to save logs and will be periodically flushed by debug probe
- Or use ITM with SWO if it is available. The big advantage here, is that you may sort logs in channels (up to 32 ITM channels on STM32H723ZG), only one wire is required and it works in "debug silicon" of Cortex-M's CoreSight (only "atomically managed" by core). You will find code about how to use SWO in the prj_h7 project. Uncomment and have fun

# Conclusion and trying other configurations

This debug and development environment seems to be reduced and with a lot of constraints.... Well actually it does look that way!

But if you understood the main steps which are:
- Configure and run `gdbserver`
- Run `gdb` and connect to `gdbserver`
- Forward `printf()` to `UART`

Then you may very easily, with a lot of different equipment (different debug probe, different `gdbserver`, different board), always find a solution to set a quite powerful development environment. As those steps are always the same!!

You may also plug those programs on your preferred IDE if it allows it (VSCode can handle it).

If your colleague monopolizes the big debug probe, you still may, with those cheap tools, start a debug session anyway.

Using `gdb` in command line is way much more easy than people think. I now largely prefer it this way, as it is more stable, and I don't have a layer between me and all `gdb`'s features (like VSCode do actually). For example, I now use the open-source debuggers Black Magic Probe [Black Magic Debug](https://black-magic.org/) and Orbuculum [Orbuculum – Orbcode](https://orbcode.org/orbuculum/) to debug dual cores targets like RP2350 or to do small tracing over SWO on STM32H723ZG! I can then forward all logs from STM32H723ZG with Orbuculum to some "home-made" program, running on my laptop to watch ADC values lives on a GUI made with Python, or do FFT on those ADC values, etc... 

### Other examples

I started with an STM32H723ZG Nucleo board, `openOCD`, serial over `UART` and `ST-Link V3`, but many other options are possible, just change one of the 4 ingredients for one you like more, or is available.

I had the following configuration and they all worked:

| Core       | Target    | debug probe              | gdb-server                    |
| ---------- | --------- | --------------------- | ----------------------------- |
| Cortex M33 | RP2350    | Raspberry Debug Probe | openOCD                       |
|            |           | Raspberry Debug Probe | openOCD                       |
| Cortex M0+ | RP2040    | JLink EDU             | openOCD / JLinkGDBServerCLExe |
|            |           | Black Magic Probe     | openOCD                       |
|            |           | STLink                | openOCD                       |
| Cortex M4  | STM32F411 | JLink EDU             | openOCD / JLinkGDBServerCLExe |
|            |           | Black Magic Probe     | openOCD                       |
|            |           | STLink                | openOCD / st-util             |
| Cortex M7  | STM32H723 | JLink EDU             | openOCD / JLinkGDBServerCLExe |
|            |           | Black Magic Probe     | openOCD                       |

**For more information about:**
- How configure `openOCD` and find correct configuration files
- How to use Black Magic Debug probe
- How to use external debug probes on Nucleo board
- How to install GNU-ARM toolchain
- How to configure VSCode to run debug session

Please look at the program examples in our github here: [link to github]

---
### Further literature for those interested (optional)

#### About `gdbserver`:

From Marus's blog ([Marus/cortex-debug: Visual Studio Code extension for enhancing debug capabilities for Cortex-M Microcontrollers](https://github.com/Marus/cortex-debug)):

> You may know them as OpenOCD, pyOCD, JLinkGDBServer, STLinkServer, etc. GDB by itself does not know anything about your device. It does know what an ARM architecture looks like but has no clue about much else. It relies on a program called a gdbserver that provides that knowledge and services. Among services, the **gdbserver helps create breakpoints**. GDB knows the instruction address of the breakpoint but does not know how to create a breakpoint in the device (especially for a FLASH-based device, which registers to set, etc.) **GDB does NOT know how to load** (program) the device with the executable file. **It asks the gdbserver to load** the program for it by giving the gdbserver address(es) and relevant data.
> [...]
> gdbservers are typically enhanced by device vendors by providing their specific algorithms and methods. gdbserver typically communicates with hardware with USB/Serial/other connections

#### `gdb` commands

To see what kind of command you may execute, look at gdb documentation. You may find some interesting commands in [GDB Commands — Black Magic Debug documentation](https://black-magic.org/usage/gdb-commands.html) and some other in [GDB Documentation](https://www.sourceware.org/gdb/documentation/)

#### `.elf` files

No, those are not Legolas files, they stand for **E**xecutable and **L**inkable **F**ormat. They are quite handy when debugging bare-metal project because they contain all symbols (you must compile with `-ggdb3` option) and path to source code on the machine you compiled it.

So during debug session, reading this .elf file, `gdb` will be able to point to correct source file and provide info about C/C++ code where Program Counter is.

For more information, look here: [Executable and Linkable Format - Wikipedia](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)
And here: [ARM ELF File Format](https://developer.arm.com/documentation/dui0101/latest/)

