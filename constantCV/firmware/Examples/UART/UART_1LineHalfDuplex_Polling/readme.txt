示例目的：
          演示UART单线半双工通信。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. UART的传输时钟设置为PCLK

演示说明：
          UART1、UART2都是单线半双工模式，双方通过一根线互传数据。

硬件连接：
          UART1_TXD (PA6) -- UART2_TXD(PB5)，用杜邦线连接
	  J1模块：EVDD、VDDIN跳线帽连接，PB05、PA06分别接到J1模块的SCL、SDA，用来提供外部上拉

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
