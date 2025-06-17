示例目的：
          将C库的printf函数重定向到UART。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK

演示说明：
          使用UART在串口调试助手上输出printf调试信息。

硬件连接：
          UART2_TXD (PB5) -- PCRX
          UART2_RXD (PB6) -- PCTX  

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
