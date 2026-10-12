
# Configure debugging chain on VSCode

>[!IMPORTANT]
>As I struggled to configure `launch.json` (see below) I only configured VSCode to work with STLink and `openOCD` as `gdb-server`. For other debugger, good luck with configuring `launch.json`.

I configured VSCode to interface itself with with `gdb-server`, using `gdb` and allowing me a more suitable GUI interface to debug.

I selected the plug-in **_Cortex-Debug_** from Marus25 because it is quite democratized and used amongst other VSCode users. 
- After installing it, a `.vscode/launch.json` file has to be configured
- Also, download the `STM32H723.svd` file (System View Description) from STMicroelectronics at the CAD page of STM32H7's official website:  [STM32H723/733 - STMicroelectronics](https://www.st.com/en/microcontrollers-microprocessors/stm32h723-733.html#cad-resources). It provides more info about memory, how registers are organized and so on.

Configuring the `launch.json` is the most horrible thing to do as there is thousands of parameters and poor documentation about those parameters is to be found.

After multiple tries and smoking all transistors of some AI, I got a working `launch.json`:
```json
   "version": "0.2.0",
   "configurations": [
      {
         "type": "cortex-debug",
         "name": "prj_h7_debug_openOcd",
         "cwd": "${workspaceRoot}/prj_h7/",
         "executable": "${workspaceFolder}/prj_h7/bin/prj_h7.elf",
         "request": "launch",
         "servertype": "openocd",
         "configFiles": [
            "${workspaceFolder}/prj_h7/stm32h7_debug.cfg"
         ],
         "searchDir": [],
         "runToEntryPoint": "main",
         "showDevDebugOutput": "none",
         "interface": "jtag", // swd also supported
         "svdFile": "${workspaceFolder}/prj_h7/STM32H723.svd",
         "device": "STM32H723ZGTx"
      }
   ]
```

>[!DANGER] Launch.json magic 
>How to configure this file is on my opinion poorly explained and I just it got working out of nowhere. No idea why. I took first some examples from the Internet, fought my ass off with this plugin's official documentation as explained before. Tried my best but no logic here... At the end I right clicked on the launch.json file, selected new Cortex-M debug session from the poped menu, adapted some path files and it worked.
>I mean, there is no explanation on how and which synthax to use for each parameter, what it exactly does and so on. It just ends in random tries...
