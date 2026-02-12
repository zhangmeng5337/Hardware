# LORA网关与采集终端硬件规格书
* 供电范围：DC5-12V
* cpu：stm32frgt6
* lora和nbiot模块使用时需要配置天线
* 设计中有两个lora模块，可以增加数据吞吐量，实际使用中只使用一个即可，默认使用M1
* lora模块使用方法以及规格书[E22-400T30S](http://www.ebyte.com/data-download.aspx?id=441&pid=33#load)
* nbiot模块使用方法以及规格书[sim7080G](https://cn.simcom.com/product/SIM7080G.html)

 **如无特殊说明，以下描述中提到的lroa1均指LORA主模块 M1,lora2代表M2**
 
## 硬件引脚功能定义
|引脚号|功能|备注|
|---|---|---|
|pc2|3.3v电源使能，高电平使能|外围电路电源使能|
|**rs485硬件资源**|||
|串口5（PC12,PD2）|485接口||
|PB12|rs485收发使能接口，低电平收数据使能||
|**lora1硬件资源**|||
|串口2（PA2,PA3）|与LORA1通信收发数据||
|PB7|lora1 M0||
|PB4|lora1 M1||
|PB3|lora1 aux||
|**lora2硬件资源**|||
|串口6（PC6,PC7）|与LORA2通信收发数据||
|PB8|lora2 M0||
|PB6|lora2 M1||
|PB5|lora2 aux||
|**nbiot硬件资源**|||
|串口1（PA9,PA10）|与NBIOT通信收发数据||
|串口3（PB10,PB11）|与gps通信||
|PC3|NBIOT开关机信号|
|PC1|与nbiot DTR相连接|备用|
|PC11|与nbiot RI相连接|备用|
|**外设控制接口**|||
|PA1|||
|PC9|||
|PA7|||
|PC4|||
|PB15|||
|PC5|||
|PB0|||
|PB1|||
|PC8|||
|PB2|||
|PB13|||
|PB14|||
|PA4|拨码开关信号1||
|PA5|拨码开关信号2||
