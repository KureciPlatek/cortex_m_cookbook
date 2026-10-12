
Black Magic Probe is an open source debugger developed by 1BitSquared and it provides quite some features!

[Black Magic Probe V2.3 – 1BitSquared](https://1bitsquared.com/products/black-magic-probe)

It has integrated:
- UART input and forward to USB interface as a virtual COM port
- integrated gdb-server: you don't have to start openOCD or any `gdb-server`!
- SWO line
- JTAG or SWD interface
- Target auto-detection, no need to hunt down target scripts/configs
## Debug with Black Magic Probe (BMP)

## Install BMP

If running on the STM32H723ZG Nucleo Board, please look first at how to use Nucleo board with an external debugger, either in user manual UM2407 from STMicroelectronics: [STM32H7 Nucleo-144 boards (MB1364) - User manual](https://www.st.com/resource/en/user_manual/um2407-stm32h7-nucleo144-boards-mb1364-stmicroelectronics.pdf), section 7.3.3 or look at [[setup_nucleoBoard]] article

>[!WARNING]
>Be sure to have modified the clock input of your project because HSE of Nucleo board is provided by STLinkV3!

After plugging it, it can be seen/detected with `dmesg` or by looking at available `/dev/` devices:

```bash
sudo dmesg
# Result:
...
[ 9473.808512] usb 3-1: new full-speed USB device number 7 using xhci-hcd
[ 9473.980821] usb 3-1: New USB device found, idVendor=1d50, idProduct=6018, bcdDevice= 2.00
[ 9473.980829] usb 3-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 9473.980832] usb 3-1: Product: Black Magic Probe v2.0.0
[ 9473.980835] usb 3-1: Manufacturer: Black Magic Debug
[ 9473.980837] usb 3-1: SerialNumber: E2E1B4DF
[ 9474.427443] cdc_acm 3-1:1.0: ttyACM0: USB ACM device
[ 9474.430059] cdc_acm 3-1:1.2: ttyACM1: USB ACM device

# In /dev:
ls /dev/ttyBmp*
# Listed devices:
/dev/ttyBmpGdb
/dev/ttyBmpGdbE2E1B4DF
/dev/ttyBmpTarg
/dev/ttyBmpTargE2E1B4DF
```

So it is good, `BMP` is there, ready to rock.

>[!WARNING] Update of `udev` rules
>**If not listed** in `/dev/tty*`, it means that `udev` rules were not updated on your Linux machine. To solve this problem, please look at: [blackmagic/driver at main · blackmagic-debug/blackmagic · GitHub](https://github.com/blackmagic-debug/blackmagic/tree/main/driver#99-blackmagic-plugdevrules)

## Use BMP

As said, Black Magic Probe integrate a `gdb-server`, which means you don't need to start one! Instead, run first and directly `gdb`, then instead of connecting to the extended-remote localhost, connect to `ttyBmpGdb` device.

Steps:
- Step 1: run `gdb`
- Step 2: In `gdb`, connect to target extended-remote `ttyBmpGdb`
- Step 3: Scan for available targets (available CPU seen by Black Magic probe)
- Step 4: attach to one of the core

Then you should be able to run your debug session

```bash
# --- STEP 1: Run gdb with the elf file:
arm-none-eabi-gdb path/to/your/prj_h7.elf

# --- STEP 2: Connect to Black Magic Probe:
(gdb) target extended-remote /dev/ttyBmpGdb
Remote debugging using /dev/ttyBmpGdb

# --- STEP 3: scan for available targets:
(gdb) monitor auto_scan
Target voltage: 3.3V
Available Targets:
No. Att Driver
 1      STM32H723 M7

# --- STEP 4: Attach to our only target. 
# In case of an STM32H7 with dual core (auxiliary Cortex-M4), two targets would be seen.
(gdb) attach 1
Attaching to program: /home/jeremie/work/prj_blog/prj_h7/bin/prj_h7.elf, Remote target
_txe_queue_receive (queue_ptr=0x240003d4 <MsgQueueTwo>, destination_ptr=0x24000a08 <tx_byte_pool_buffer+1528>, wait_option=0)
    at /home/jeremie/work/threadx/common/src/txe_queue_receive.c:101
101         else if (queue_ptr -> tx_queue_id != TX_QUEUE_ID)

# Run your debug session: flash and run program
(gdb) load
Loading section .isr_vector, size 0x2cc lma 0x8000000
Loading section .text, size 0x1e7dc lma 0x80002d0
Loading section .rodata, size 0x21c lma 0x801eaac
Loading section .ARM, size 0x8 lma 0x801ecc8
Loading section .init_array, size 0x4 lma 0x801ecd0
Loading section .fini_array, size 0x4 lma 0x801ecd4
Loading section .data, size 0xcc lma 0x801ecd8
Start address 0x0801ea3c, load size 126368
Transfer rate: 18 KB/sec, 936 bytes/write.

# All good, let's roll
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/jeremie/work/prj_blog/prj_h7/bin/prj_h7.elf

```

>[!INFO]
>I didn't try to start a debug session with VSCode as it is sometimes very catastrophic to configure the launch.json, and just randomly try until it works is not my art of working. But feel free to try it.

## Use BMP UART interface for `printf()`

Actually there is almost nothing to explain here, just plug the UART interface provided by BlackMagic Probe to the UART3 of Nucleo board, any place should be good, no need to solder anything.

Then 