# 地磁NODE测试方法
* 所有串口波特率为115200，8N1

## 烧录代码

使用stlink仿真器分别给地磁板子上面的两个cpu以及基站板子的两个cpu烧录最新代码
stlink的使用方法可以自行查阅，使用stlink utility软件烧录即可

* 具体接线方式如下
![](/Users/admin/Downloads/WX20200106-103206@2x.png)

## 获取地磁实时数据

* 为了优化算法，需要在前期测试获取实时数据，具体方法如下（任选其一） 

  * 通过串口线连接电脑，在电脑端用串口调试助手存储实时地磁数据 
  * 购买串口sd卡记录仪记录地磁数据[串口数据记录仪链接](https://item.taobao.com/item.htm?spm=a230r.1.14.36.4ef416e0lVAnyZ&id=596300748339&ns=1&abbucket=4#detail)
* 连线方法

上述两种测试方法接线方式基本相同，其中串口sd卡记录仪需要通过VCC供电

## 无线测试

使用micro usb数据线连接基站pcb，然后利用串口调试软件接收数据即可
 ![](/Users/admin/Downloads/WechatIMG225.jpeg)