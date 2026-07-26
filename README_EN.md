## 1. Project Introduction

### 1.1 Overview

This project is derived from the UAV tutorial of Shangguigu. I have iterated and debugged based on the original prototype, and made some **logical and functional optimizations** as well as added **new features** to make it more understandable and easier to use. To improve the readability of the code, I have added **extensive comments and explanations** throughout the project to facilitate reading and understanding by other developers.

***This project is still under development, and future iterations and optimizations will be made according to actual requirements.***

[Original Tutorial Link](https://www.bilibili.com/video/BV1f8rbBSEq3?spm_id_from=333.788.videopod.episodes&vd_source=62df64ed7202909f5eecb7105fd5eb73*)

### 1.2 Project Structure

```
Drone-project-based-on-STM32
|
|---> P01_flight_hal     (Flight controller part) ---> Core (CubeMX auto-generated application initialization code)
|                                       |       |---> Inc (header files)
|                                       |       |---> Src (source files)
|                                       |
|                                       |---> Drivers (official low-level libraries)
|                                       |       |---> CMSIS (ARM CMSIS standard core library)
|                                       |       |---> STM32F1xx_HAL_Driver (STM32F1 HAL hardware abstraction layer drivers)
|                                       |
|                                       |---> MDK-ARM (Keil5 dedicated project directory)
|                                       |       |---> .vscode (VS Code editor configuration)
|                                       |       |---> application (application-level business code, FreeRTOS task logic)
|                                       |       |---> common (common modules, debug prints, global config)
|                                       |       |---> DebugConfig (Keil debugger configuration)
|                                       |       |---> FreeRTOS (FreeRTOS real-time OS source code)
|                                       |       |---> interface (hardware interface driver layer)
|                                       |       |       |---> int_led (LED indicator driver)
|                                       |       |       |---> int_motor (motor PWM driver)
|                                       |       |       |---> int_SI24R1 (SI24R1 2.4G wireless chip driver)
|                                       |       |       |---> int_IP5305T (IP5305T power management chip driver)
|                                       |       |---> P01_flight_hal (project output directory, includes .sct scatter-loading script)
|                                       |       |---> P01_flight_hal.uvprojx (Keil5 project main file)
|                                       |       |---> startup_stm32f103xb.s (STM32F103 startup assembly file)
|                                       |
|                                       |---> P01_flight_hal.ioc (CubeMX graphical configuration project source)
|
|---> P02_remote_hal      (Remote controller part) ---> Core (CubeMX auto-generated application initialization code)
|                                       |       |---> Inc (header files)
|                                       |       |---> Src (source files)
|                                       |
|                                       |---> Drivers (official low-level libraries)
|                                       |       |---> CMSIS (ARM CMSIS standard core library)
|                                       |       |---> STM32F1xx_HAL_Driver (STM32F1 HAL hardware abstraction layer drivers)
|                                       |
|                                       |---> MDK-ARM (Keil5 dedicated project directory)
|                                       |       |---> .vscode (VS Code editor configuration)
|                                       |       |---> application (application-level business code, FreeRTOS task logic)
|                                       |       |---> common (common modules, debug prints)
|                                       |       |---> DebugConfig (Keil debugger configuration)
|                                       |       |---> FreeRTOS (FreeRTOS real-time OS source code)
|                                       |       |---> interface (hardware interface driver layer)
|                                       |       |       |---> int_SI24R1 (SI24R1 2.4G wireless chip driver)
|                                       |       |       |---> int_IP5305T (IP5305T power management chip driver)
|                                       |       |---> P02_remote_hal (project output directory, includes .sct scatter-loading script)
|                                       |       |---> P02_remote_hal.uvprojx (Keil5 project main file)
|                                       |       |---> startup_stm32f103xb.s (STM32F103 startup assembly file)
|                                       |
|                                       |---> P02_remote_hal.ioc (CubeMX graphical configuration project source)
|
|---> PCB_files(PCB hardware guide files)
|---> .gitignore (Git ignore rules)
|---> README.md (Project documentation - Chinese)
|---> README_EN.md (Project documentation - English)
```

## 2. Project Execution

### 2.1 Environment Preparation

**Keil MDK5 (Required)**

STM32CubeMX (Optional, for code generation during self-debugging)

VS Code (Optional, for improved code editing experience)

### 2.2 Execution Steps

# Quick Start Guide

This guide is intended for users who use **only the Keil MDK‑ARM** development environment, without the need to install STM32CubeMX or other IDEs.

---

## 1. Environment Preparation

### 1.1 Install Keil MDK‑ARM (if not already installed)

- Install [Keil MDK‑ARM](https://www.keil.com/download/product/) (version 5.0 or higher).
- During installation, ensure that the **STM32F1 series support pack** is checked, or install it manually later via the Pack Installer:
  - Open Keil and click the `Pack Installer` button.
  - In `Devices`, search for `STM32F103` (the specific chip used in this project is STM32F103C8T6), then click `Install` to install the `Keil.STM32F1xx_DFP` pack.

### 1.2 Hardware Preparation

- **Official purchase link**: [Drone body + Remote controller](https://e.tb.cn/h.8dxjhQnxAMEf27j?tk=UyxHgFQpvYE)

- **Custom PCB fabrication and soldering**: Open the PCB folder, order boards according to the Gerber files (recommended using JLCPCB order assistant), purchase components according to the BOM, and complete soldering yourself.

## 2. Building the Projects

This project contains two independent projects that must be built separately:

- `P01_flight_hal` — **Flight controller**
- `P02_remote_hal` — **Remote controller**

### 2.1 Open the Project
1. Navigate to the corresponding project directory:
   - Flight controller: `Drone-project-based-on-STM32-main/P01_flight_hal/MDK-ARM/`
   - Remote controller: `Drone-project-based-on-STM32-main/P02_remote_hal/MDK-ARM/`
2. Double-click `P01_flight_hal.uvprojx` or `P02_remote_hal.uvprojx`, and Keil will automatically open the project.

### 2.2 Select Build Target
- In the Keil toolbar, from the `Target` dropdown, confirm that the target name corresponding to **`P01_flight_hal`** (flight controller) or **`P02_remote_hal`** (remote controller) is selected (usually already selected by default).

### 2.3 Build
- Click the **`Build`** button on the toolbar (or press `F7`) to start compilation.
- If the build succeeds, the `Build Output` window at the bottom will show **`"0 Error(s), 0 Warning(s)"`**, and the `.axf` and `.hex` files will be generated (located in the `MDK-ARM/P01_flight_hal/` or `P02_remote_hal/` output directory).

---

## 3. Flashing Firmware

### 3.1 Connect the Programmer
- Connect the ST‑Link / J‑Link to your computer's USB port, and connect the other end to the **SWD** interface on the flight controller / remote controller board (usually SWCLK, SWDIO, 3.3V, GND).
- **Power on** the flight controller / remote controller board (using battery power: flight controller battery model 603048 +3.7V 2000mAh, remote controller battery 902731 3.7V 500mAh).

### 3.2 Configure the Programmer
1. In Keil, click **`Options for Target`** (the magic wand icon) or press `Alt+F7`.
2. Switch to the **`Debug`** tab:
   - In the `Use` dropdown on the right, select your programmer (e.g., `ST-Link Debugger`).
   - Click `Settings` and confirm that the chip is correctly recognized (showing `STM32F103x` series).
3. Switch to the **`Utilities`** tab:
   - Ensure `Use Debug Driver` is checked, and select the same programmer as in Debug.
   - Click `Settings`, and in `Programming Algorithm`, add `STM32F10x Med-density Flash` (if already present, no action needed).

### 3.3 Perform Flashing
- Click the **`Load`** button on the toolbar (or press `F8`) to start downloading.
- After successful flashing, `Build Output` will display **`"Application running..."`** or a completion message.

---

## 4. Running and Debugging

### 4.1 First Run
- After flashing, the board will automatically reset and run the firmware.
- If it does not run automatically, manually press the **reset button** on the board (if available), or power cycle the board.

### 4.2 Serial Debugging (Optional)
- The project uses the debug print functionality from the `common` module (via serial port).
- Connect a USB-to-serial module to the board's **USART1** (TX/RX), with baud rate default **115200**.
- Open a serial terminal (e.g., PuTTY, SecureCRT) to view the flight controller / remote controller logs.

### 4.3 Debug Mode
- Click **`Start/Stop Debug Session`** (the magnifying glass icon) to enter debug mode.
- You can set breakpoints, single-step execute, inspect variables and registers, which helps with troubleshooting.

---

## 5. Important Notes

- **Two independent projects**: The flight controller and remote controller must be compiled and flashed separately; they communicate via the 2.4G wireless module (SI24R1).
- **Power management**: If using the IP5305T power management chip, ensure the battery voltage is normal; otherwise, the flight controller may not start.
- **FreeRTOS configuration**: The projects use FreeRTOS by default; task priorities and stack sizes are pre-configured. Do not modify them unless necessary.
- **If compilation errors occur**:
  - Check that the STM32F1 support pack is installed in Keil.
  - Check that the project path does not contain Chinese characters or spaces (which may cause compiler issues).
  - Try performing `Rebuild All` to clean intermediate files.

---

## 6. Frequently Asked Questions

**Q: "No Cortex-M SW Device Found" when flashing**  
A: Check that the SWD wiring is correct, the target board is powered, and the programmer driver is installed successfully.

**Q: No response after flashing**  
A: Confirm that power is normal and that the BOOT0 pin is grounded (to boot from Flash). If using ST‑Link, try lowering the SWD clock frequency in the debug settings.

**Q: No serial output**  
A: Check that the serial connection is correct (TX → RX, RX → TX, GND connected) and the baud rate matches the code (default 115200).

### 2.3 Execution Results

***To be continued...***