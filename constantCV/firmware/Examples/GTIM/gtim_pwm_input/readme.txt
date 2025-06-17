示例目的：
          演示GTIM的PWM输入捕获功能

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，12分频，48MHz/12=4MHz，PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=4MHz
          4. GTIM1不分频        
          5. 设置PA06为GTIM1的CH1通道， GTIM1不分频，同一通道输入信号被两路捕获寄存器进行捕获。

演示说明：
           使用GTIM1的CH1单个通道对PWM输入信号进行测量,CCR1寄存器测量周期，
           CCR2寄存器测量脉宽。将PWM输入信号引入到PA06上。
      
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