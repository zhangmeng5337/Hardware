示例目的：
         利用I2C接口， 实现I2C主设备功能，和I2C从机进行数据通信，当从机芯片供电电压较低，如：1.8V时。

硬件资源：
          1. CW32L010 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟2分频，24MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. PB05复用为SDA PB06复用为SCK
          5. VC1正端输入为PB05，负端为Vcore/2，VC2正端输入为PB06，负端为Vcore/2，VC1_OUT作为I2C的SDA，VC2_OUT作为I2C的SCL，VC起到电平转换的作用

演示说明：
1、采用默认时钟配置，初始化I2C为主接口，100KHz，不使能I2C中断。
2、主从通信从设备I2C接收时序：根据I2C模块状态变化，接收保存数据并回应ACK，直到主动回应NACK或者收到STOP信号。
3、主从通信从设备I2C发送时序：根据I2C模块状态变化，发送数据，直到收到NACK信号或者STOP信号。
4、程序流程：程序完成I2C主设备配置后，先将u8Senddata数组中的内容发给从机：发送START信号后，I2C模块会产生状态改变，程序持续查询I2C模块状态并根据不同状态值进行不同处理，直到完成SLA+W、所有待发送的数据字节以及STOP信号的发送。然后启动数据接收流程：发送启动信号后，I2C模块会产生状态改变，程序持续查询I2C模块状态并根据不同状态值进行不同处理，直到完成SLA+R、所有待接收字节的接收，以及STOP信号发送
5、注意实际测试时需要更改I2C/GPIO口线、时钟配置以及GPIO配置都需要修改。
6、注意主从通信时，需要连接从机单板对应I2C总线到单板的EEPROM接口上(标号CN11,VDDE/SCL/SDA三根口线都要连接，以给I2C提供上拉电阻)
7、I2C从机设备地址目前统一在cw32f010_i2c.h文件中进行定义，在来回测试EEPROM读写和两块单板进行主从通信时需要注意修改。
8、VC电平转换的原理：当主机I2C向低电压从机发送数据时，PB06发送100K时钟，PB05向从机发送数据，VC1，VC2不影响主机的发送，但是当低电压从机向主机回NACK或者发送高电平时，PB05接收到从机发送的高电平，实际上不到3.3V，如：1.8V，这时VC1就会将从机发过来的电平转换成主机的高点平3.3V，确保通信数据不受从机低电平的影响

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开i2c_cw24c02_int.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
