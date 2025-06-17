示例目的：
          LIN总线应用。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK

硬件连接：
          UART2_TXD (PB5) --TJA1021 RXD
          UART2_RXD (PB6) -- TJA1021 TXD 
          PB03 -- SLP
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
