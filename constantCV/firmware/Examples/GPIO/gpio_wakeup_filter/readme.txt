示例目的：
          演示GPIO中断唤醒功能

硬件资源：
          1. CW32L010FxPx StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟12分频，4MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4.  PA06输入， PB02输出

演示说明：
        KEY2按下后，将系统从休眠中唤醒，同时LED2也会随之进行亮灭。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)