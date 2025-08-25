# myapp 示例集合

本文件夹用于存放基于 Zephyr RTOS 开发的个人应用示例。每个子目录代表一个独立的应用，均可在支持 Zephyr 的硬件平台上编译和运行。

## 目录结构

```
myapp/
├── random/
│   ├── src/
│   ├── prj.conf
│   └── CMakeLists.txt
├── user_button/
│   ├── src/
│   ├── prj.conf
│   └── CMakeLists.txt
├── LM35/
│   ├── src/
│   ├── prj.conf
│   └── CMakeLists.txt
├── Rheostat/
│   ├── src/
│   ├── prj.conf
│   └── CMakeLists.txt
├── DHT11/
│   ├── src/
│   ├── prj.conf
│   └── CMakeLists.txt
└── ...
```

## 应用介绍

### random

[`random`](random/) 是一个简单的 Zephyr 应用示例，每秒会执行以下操作：

- 闪烁开发板上的 LED（验证 GPIO 配置和控制）
- 通过串口打印一个随机的 uint32 数值和一个 double 浮点数（验证串口通信、随机数生成和浮点数打印功能）
- 如果在 Kconfig 中启用自定义模块 `say_hello`，则每秒还会通过串口输出一条来自该模块的问候信息（验证自定义模块集成和 Kconfig 配置功能）
- 支持用户按键（user button），每当按键状态变化时会切换另一个 LED 的状态（验证按键输入和 LED 控制功能）

该示例可用于快速验证串口、LED、随机数、浮点数打印以及 Kconfig 配置和自定义模块集成等基础功能是否正常，是后续开发的良好基础

### user_button

[`user_button`](user_button/) 是一个基于 uno3 兼容扩展板的 Zephyr 应用示例。该扩展板通过 overlay 文件 [`uno3_shield.overlay`](user_button/boards/uno3_shield.overlay) 添加了两个按键（Shield D2、D3）和三个 LED（Shield D9、D10、D11）。在 `user_button` 应用中，主程序支持这两个按键和三个 LED 的控制：

- 按下 Shield D2 或 D3 按键时，分别切换对应 LED 的状态
- 可以通过串口观察按键和 LED 状态的变化

该示例可用于验证自定义硬件资源（按键和 LED）的集成与控制

### LM35

[`LM35`](LM35/) 应用用于演示如何通过 ADC 采集 LM35 温度传感器的模拟信号，并通过串口输出温度值。主要功能：

- 配置 ADC 通道（PC3 = ADC1_IN13）
- 每秒采集一次温度数据
- 目前看来温度数据并不是很准确

该示例可用于验证 ADC 的使用

### Rheostat

[`Rheostat`](Rheostat/) 应用用于演示如何通过 ADC 采集电位器（滑动变阻器）的模拟信号，并通过串口输出采样值。主要功能：

- 配置 ADC 通道（PB1 = ADC1_IN9）
- 每秒采集一次电位器数据

该示例可用于验证 ADC 的使用

### DHT11

[`DHT11`](DHT11/) 应用用于演示如何每3秒采集一次 DHT11 温湿度传感器的数据，并通过串口输出结果。主要功能：

- 使用 menuconfig enable DHT driver
- 每3秒自动采集一次温度和湿度数据

该示例可用于验证对内置 driver 的使用

## 使用方法

1. 进入对应应用目录，例如 `random`、`LM35`、`Rheostat` 或 `DHT11`。
2. 使用 west 或 cmake 命令进行编译，例如：

   ```sh
   west build -p always -b stm32f469i_disco -t menuconfig # open the menuconfig, like enable random and float print
   west build -p always -b stm32f469i_disco  # build the project, the output can be found under build/zephyr/zephyr.bin

   or with custom overlay file

   west build -p always -b stm32f469i_disco -t menuconfig -- -DDTC_OVERLAY_FILE="boards/uno3_shield.overlay"
   west build -p always -b stm32f469i_disco -- -DDTC_OVERLAY_FILE="boards/uno3_shield.overlay"
   ```

3. 按照各应用目录下的说明进行烧录和运行。

    ```sh
    #1)install usbipd-win on Windows
    #In PowerShell (Admin):
    winget install usbipd

    #2) install flashing tools inside WSL
    #Open your WSL distro:
    #Ubuntu/Debian:
    sudo apt update
    sudo apt install stlink-tools openocd usbutils

    #3) pass the ST-Link into WSL
    #Plug the STM32F469I-DISCO (its on-board ST-Link) into USB.
    #In PowerShell (Admin) on Windows:
    usbipd list

    #Find the ST-Link row and note the BUSID (looks like 3-5).
    #Bind once (saves policy):
    usbipd bind --busid 3-5

    #Attach to WSL (while your WSL shell is open):
    usbipd attach --wsl --busid 3-5

    #4) Back in WSL, verify:
    lsusb
    #You should see STMicroelectronics ST-LINK/V2.x.

    #5) flash the firmware
    sudo st-info --probe
    sudo st-flash --reset write your_firmware.bin 0x08000000

    #6) detach the ST-Link from WSL When you’re done:
    usbipd detach --busid 3-5
    ```

4. 测试

    ```sh
    python -m serial.tools.miniterm "COM6" 115200
    ```

    串口终端输出示例：

    **random 应用：**
    ```
    LED state: ON
    Random value: 1943892486
    Random double: 1.452598
    Hello, World! From say_hello modules.
    LED state: OFF
    Random value: 3434347776
    Random double: 1.799621
    Hello, World! From say_hello modules.
    ...
    ```

    **Rheostat 应用：**
    ```
    [00:01:50.011,000] <inf> Rheostat: Sample=1
    [00:01:54.011,000] <inf> Rheostat: Sample=773
    [00:01:57.011,000] <inf> Rheostat: Sample=1104
    [00:01:58.011,000] <inf> Rheostat: Sample=3219
    ...
    ```

    **LM35 应用：**
    ```
    [00:00:36.003,000] <inf> lm35_app: Sample=560, mV=451, LM35 temperature=45.1 °C
    [00:00:37.003,000] <inf> lm35_app: Sample=559, mV=450, LM35 temperature=45.0 °C
    ...
    ```

    **DHT11 应用：**
    ```
    Temp: 27.000000 °C, Hum: 56.000000 %
    Temp: 27.000000 °C, Hum: 56.000000 %
    ...
    ```

## 依赖

- WSL2 (to be able to download FW under Windwos on ARM platform)
- Testing with stm32f469i_disco develop kits

## 参考

- [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)
- [Zephyr Samples](https://docs.zephyrproject.org/latest/samples/index.html)