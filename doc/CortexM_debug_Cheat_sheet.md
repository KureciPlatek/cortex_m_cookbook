
# Combinaisons
## STM32H7 + openOCD + STLink

```bash
# Terminal 1 (gdb-server)
openocd -f ~/work/prj_blog/prj_h7/stm32h7_debug.cf

# Terminal 2 (gdb)
arm-none-eabi-gdb -iex "add-auto-load-safe-path ./" ./bin/*.elf
(gdb)
```

## STM32H7 + BMP


```bash
# Terminal 1 (gdb-server)
ls /dev/tty*
# Result:
/dev/ttyBmpGdb
/dev/ttyBmpGdbE2E1B4DF
/dev/ttyBmpTarg
/dev/ttyBmpTargE2E1B4DF

# Terminal 2 (gdb)
arm-none-eabi-gdb ./bin/*.elf
(gdb) target extended-remote /dev/ttyBmpGdb
(gdb) monitor auto_scan
(gdb) attach 1
```


## STM32H7 + st-util + STLink


```bash
# Terminal 1 (gdb-server)
st-util

# Terminal 2 (gdb)
arm-none-eabi-gdb -iex "add-auto-load-safe-path ./" ./bin/*.elf
(gdb)
```

## STM32H7 + JLink + JLinkGDBServerCLExe


```bash
# Terminal 1 (gdb-server)
JLinkGDBServerCLExe -if SWD -device STM32H723ZG

# Terminal 2 (gdb)
arm-none-eabi-gdb ./bin/*.elf
(gdb) target extended-remote :2331
```

## STM32H7 + JLink + openOCD

Modify `stm32h7_debug.cfg` to select jLink:

```cfg
source [find interface/jlink.cfg]
# source [find st_interface/stlink-dap.cfg]
```

(use `stm32_debug_jlink.cfg` file)

```bash
# Terminal 1 (gdb-server)
openocd -f ~/work/prj_blog/prj_h7/stm32h7_debug_jlink.cf

# Terminal 2 (gdb)
arm-none-eabi-gdb -iex "add-auto-load-safe-path ./" ./bin/*.elf
(gdb)
```

## STM32F4 + openOCD + STLink

```bash
# Terminal 1 (gdb-server)
openocd -f ~/work/prj_blog/prj_f4/stm32f4_debug.cf

# Terminal 2 (gdb)
arm-none-eabi-gdb -iex "add-auto-load-safe-path ./" ./bin/*.elf
(gdb)
```


## STM32F4 + BMP


```bash
arm-none-eabi-gdb prj_f4.elf
(gdb) target extended-remote /dev/ttyBmpGdb
(gdb) monitor auto_scan
(gdb) attach 1
```

Serial `printf()`'s are available with `minicom -D /dev/ttyACM2` 


## STM32F4 + JLink + JLinkGDBServerCLExe

Run gdb-server:

```bash
JLinkGDBServerCLExe -if SWD -port 3333 -device STM32F411RE
```

Then connect as always

## RP2350 + Raspberry Debug Probe


```bash
# Terminal 1 (gdb-server)
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg -c "adapter speed 5000"

# Terminal 2 (gdb)
arm-none-eabi-gdb -iex "add-auto-load-safe-path ./" ./bin/*.elf
(gdb)

```





# Helpers

## Raspberry Pi 5 connect to WiFi

```bash
#   Add new WiFi:
sudo nmcli d wifi list
sudo nmcli d wifi connect "<SSID>" password "<password>"
   
#   Hotspot:
nmcli d wifi show-password
```

## Raspberry Pi 5 create hotspot

```bash
sudo nmcli device wifi hotspot ssid rpi5_hotspot password rp15_h075p07 ifname wlan0
```
