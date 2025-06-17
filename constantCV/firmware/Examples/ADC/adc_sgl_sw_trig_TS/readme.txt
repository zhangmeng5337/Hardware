示例目的：
          演示软件启动ADC单通道转换模式，采样芯片温度。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSI
          3. 系统时钟源为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ADC的时钟源为PCLK，8分频
          5. PB02(LED2)设置为PUSH-PULL输出

演示说明：
          ADC通道为BGR_TS。
          ADC转换完成，PB02输出电平翻转, LED2闪烁。
          支持查询模式和中断模式，获取采样结果
         

使用说明：
+ EWARM
          1. 打开adc_sgl_sw.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开adc_sgl_sw.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)