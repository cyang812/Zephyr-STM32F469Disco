# myapp 示例集合

本文件夹用于存放基于 Zephyr RTOS 开发的个人应用示例。每个子目录代表一个独立的应用，均可在支持 Zephyr 的硬件平台上编译和运行。

## 目录结构

```
myapp/
├── random/
│   ├── src/
│   ├── prj.conf
│   └── CMakeLists.txt
└── ...
```

## 应用介绍

### random

`random` 是一个简单的 Zephyr 应用示例，每秒会执行以下操作：

- 闪烁开发板上的 LED（验证 GPIO 配置和控制）
- 通过串口打印一个随机的 uint32 数值和一个 double 浮点数（验证串口通信、随机数生成和浮点数打印功能）

该示例可用于快速验证串口、LED、随机数和浮点数打印等基础功能是否正常，是后续开发的良好基础

## 使用方法

1. 进入对应应用目录，例如 `random`。
2. 使用 west 或 cmake 命令进行编译，例如：

   ```sh
   west build -p always -b stm32f469i_disco -t menuconfig # open the menuconfig, like enable random and float print
   west build -p always -b stm32f469i_disco  # build the project, the output can be found under build/zephyr/zephyr.bin
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

    你将会在串口终端看到如下输出，每秒刷新一次：

    ```
    LED state: OFF
    Random value: 926324701
    Random double: 1.215677
    LED state: ON
    Random value: 1181891265
    Random double: 1.275181
    LED state: OFF
    Random value: 1491022294
    Random double: 1.347156
    ```

    如果 LED 正常闪烁且串口能持续输出随机数和浮点数，说明 GPIO、串口、随机数和浮点数打印等基础功能均已正常工作

## 依赖

- WSL2 (to be able to download FW under Windwos on ARM platform)
- Testing with stm32f469i_disco develop kits

## 参考

- [Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)
- [Zephyr Samples](https://docs.zephyrproject.org/latest/samples/index.html)
