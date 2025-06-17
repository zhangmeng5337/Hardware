示例目的：
          读取数字签名相关内容。

硬件资源：
          1. CW32L010CxTx StartKit
          2. 时钟HSI
          3. 系统时钟默认HSI时钟分频到4MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=4MHz
          4. L010：PB02设置为PUSH-PULL输出，驱动LED2
             
演示说明：
          程序依次读取芯片类型/引脚数/FLASH容量/RAM容量以及芯片UID，然后通过LED2口线来持续闪灯。

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
