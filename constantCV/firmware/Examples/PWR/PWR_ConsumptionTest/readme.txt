示例目的：
          演示MCU通过按键进入深度休眠模式，再通过按键切换唤醒方式。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSI，LSI
          3. PA06设置为按键中断输入
          5. PA05配置为UART1的Rxd
          6. PB06和PB07配置为UART2的Rxd和Txd

演示说明：
        上电后，MCU以4MHz的频率运行1S，并通过UART2输出当前的运行频率，然后MCU的主频切换到8MHz运行1S，同时打印出当前主频，
        然后MCU的主频切换到16MHz运行1S，同时打印出当前主频，然后MCU的主频切换到24MHz运行1S，同时打印出当前主频，然后MCU
        的主频切换到48MHz运行1S，同时打印出当前主频，然后MCU的主频切换到4MHz运行，进入deepsleep状态。
        通过PA06的下降沿唤醒MCU，首次唤醒MCU启动IWDT然后进入deepsleep，第二次唤醒关闭IWDT，启动LPTIM，然后进入deepsleep，
        第三次唤醒关闭LPTIM，启动uart1，然后进入deepsleep，第四次唤醒，关闭uart1，启动RTC，然后进入deepsleep，第5次唤醒
        将同时启动IWDT，LPTIM，Uart1和RTC，然后进入deepsleep。第6次唤醒后，不进入deepsleep，程序等待约4s后，IWDT发生超时
        复位MCU。
        MCU在休眠时，如果被LPTIM，RTC和UART的中断唤醒，将打印出相应的唤醒源。 
         
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
