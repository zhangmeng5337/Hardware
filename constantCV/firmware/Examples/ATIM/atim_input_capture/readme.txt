示例目的：
          演示ATIM输入捕获。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，1分频，ARR设置为9.
          5. PB02设置为PUSH-PULL输出，驱动LED2
          6. PA06设置为输入，检查KEY2的输入
          7. PB03设置为输入，复用为BTIM2TOGN

演示说明：
          配置BTIM2以2MHz频率计数，使能TOG功能，通过PB03输出当前BTIM2的溢出周期。
          KEY2用于改变BTIM2的溢出周期，范围为1~13kHz，步长2kHz。
          LED2闪烁指示KEY2按下有效。
          ATIM以8MHz频率计数，配置CH1捕获BTIM2_Trgo, 上升沿有效,配置CH2捕获BTIM2_Trgo,下降沿有效.
          使能CC1和CC2的中断,在中断中处理BTIM2_Trgo信号的周期和脉宽

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
