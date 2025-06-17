示例目的：
          演示ATIM在从模式下，对外部信号通过ETR引脚计数。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，1分频，ARR设置为9.
          5. PB02设置为PUSH-PULL输出
          6. PA02设置为输入，复用为ATIM_ETR

演示说明：
           在PA02引脚上接入跳变的信号，如按键，当PA02上有上升沿时，ATIM进行一次计算，当计数次数到10次后，ATIM将产生溢出事件，并触发
          中断服务程序。中断服务程序中，PB02输出电平翻转。

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
