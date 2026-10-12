


## Debug with `JLink EDU mini`

Two ways: 
- Either run Ozone on your local machine, `JLinkRemoteServer` on the host and connect to host with Ozone, to a remote `JLink`. Very simple and sexy I must say.
- Or like always, run a `gdb-server` with `openOCD`, and execute debug session

### Debug with Ozone and `JLinkRemoteServer`

Context and situation of the different machines:

![[diagrams/jlink_remoteServer.svg]]

So the difference here is that my debug session is on my laptop. Not on the RPi5 which is the host for JLinkRemoteServer.

>[!WARNING]
>It means that you have to compile your elf file on your laptop, so that when loaded to Ozone and flashed on your target, Ozone will know where to point to: which source file, with which path. If compiled and RPi5, and ran from laptop, Ozone will tell you it can't find path to source code. 


```bash
# Run:
JLinkRemoteServer

# Output:
SEGGER J-Link Remote Server V8.74a
Compiled Oct  2 2025 18:34:54

'q' to quit '?' for help

2025-10-09 12:59:56 - Remote Server started
2025-10-09 12:59:56 - Connected to J-Link with S/N 801022472
2025-10-09 12:59:56 - Waiting for client connections...
```

On Ozone, go to `Tools` -> `J-Link Settings` -> `Host Interface` then enter the IP address of the remote machine, on which target and JLink is plugged and `JLinkRemoteServer` is working:

![[diagrams/setJLinkRemoteServer.png]]

On remote machine when laptop is connected:

```bash
# When connected with Ozone, on remote machine running JLinkRemoteServer, the following output should be seen:
2025-10-09 13:03:52 - Conn. 9: Client connected.
2025-10-09 13:03:52 - Client connected
```

Finito! Now you can have fun debug the `ThreadX` example program of `prj_h7/`

### Debug with JLink and `openOCD` as `gdb-server`

The only thing to modify is the `stm32h7_debug.cfg` file, to go fetch `interface/jlink.cfg` configuration file, and remove line `transport select "dapdirect_swd"` as it is not supported by `JLink`. No idea why, but it worked:

```tcl
...
source [find interface/jlink.cfg]
# source [find st_interface/stlink-dap.cfg] # Deactivated, be careful when debugging with STLink again

set WORKAREASIZE 0x8000

# transport select "dapdirect_swd" # Deactivated, be careful when debugging with STLink again
...
```

Start `gdb-server`:

```bash
# Run gdb-server
openocd -f ~/path/to/prj_blog/prj_h7/stm32h7_debug.cfg

# Result:
Open On-Chip Debugger 0.12.0+dev-snapshot (2025-07-16-14:15)
Licensed under GNU GPL v2
...
Info : [STM32H723ZGTx.cm7] Examination succeed
Info : [STM32H723ZGTx.ap2] gdb port disabled
Info : [STM32H723ZGTx.cm7] starting gdb server on 3333
Info : Listening on port 3333 for gdb connections
Info : [STM32H723ZGTx.cm7] external reset detected

# When gdb connected:
...
Info : JTAG tap: STM32H723ZGTx.cpu tap/device found: 0x6ba00477 (mfg: 0x23b (ARM Ltd), part: 0xba00, ver: 0x6)
Info : JTAG tap: auto0.tap tap/device found: 0x06483041 (mfg: 0x020 (STMicroelectronics), part: 0x6483, ver: 0x0)
[STM32H723ZGTx.cm7] halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x0801ce2c msp: 0x24050000
Info : Reduced speed from 8000 kHz to 4000 kHz (maximum)
Info : Padding image section 0 at 0x080002cc with 4 bytes
Info : Padding image section 1 at 0x0801eda4 with 28 bytes (bank write end alignment)
...
```

On another terminal, run `gdb`:

```bash
# Run gdb
arm-none-eabi-gdb -iex "add-auto-load-safe-path ./" ~/path/to/prj_blog/prj_h7/bin/<prog>.elf

# .gdbinit file if exists is ran:
...
Reading symbols from ./bin/prj_h7.elf...
0x0801ce2c in __udivmoddi4 (n=0, d=0, rp=0x0) at ../../../../../../libgcc/libgcc2.c:1180
warning: 1180   ../../../../../../libgcc/libgcc2.c: No such file or directory
Loading section .isr_vector, size 0x2cc lma 0x8000000
Loading section .text, size 0x1e7dc lma 0x80002d0
Loading section .rodata, size 0x21c lma 0x801eaac
Loading section .ARM, size 0x8 lma 0x801ecc8
Loading section .init_array, size 0x4 lma 0x801ecd0
Loading section .fini_array, size 0x4 lma 0x801ecd4
Loading section .data, size 0xcc lma 0x801ecd8
Start address 0x0801ea3c, load size 126368
Transfer rate: 17 KB/sec, 9026 bytes/write.

# Run target
(gdb) run
```

Worked perfectly fine, openOCD is able to run with JLink debugger and provide a `gdb-server`

