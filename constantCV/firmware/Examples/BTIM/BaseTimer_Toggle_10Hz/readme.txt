示例目的：
         演示基本定时器BTIM的TOG输出功能，不需要中断和口线寄存器的操作，完全由硬件实现方波的输出

硬件资源：
          1. CW32L010 StarKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. BTIM1的时钟设置为PCLK，8分频
          5. PA00,PA01设置为PUSH-PULL输出, 复用为BTIM1的TOG输出

演示说明：
          BTIM1以1MHz时钟进行计数，ARR寄存器为49999，则BTIM1将每50ms溢出一次，  PA00输出TOGP，PA01输出TOGN，可以示波器观测波形，输出波形为10Hz的PWM波

使用说明
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
