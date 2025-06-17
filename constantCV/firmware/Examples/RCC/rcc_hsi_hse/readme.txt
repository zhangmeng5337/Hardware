示例目的：
          系统时钟源在HSI和HSE之间进行切换。

硬件资源：
          1. CW32L010FxPx StartKit  
          2. 时钟HSI和LSE
          3. HSI作为系统时钟时，设置为HSIOSC到HSI时钟的分频比为2，则SysClk为24MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz，注意此时需要正确配置FLASH的读访问等待周期。
          4. HSE作为系统时钟时，16MHz。PCLK、HCLK不分频，PCLK=HCLK=SysClk=16MHz。

演示说明：
         程序循环在HSI和HSE时钟之间进行切换，LED2先以HSI做延时闪烁5次后，切换成HSE做时钟，LED2再闪烁次，如此循环，可明显看到使用HSI和HSE时，LED2的闪烁频率不一样。
         如果切换失败，则LED2灯会慢闪（约1S亮，1S熄）。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开rcc_hsi_hse.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
