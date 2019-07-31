精简版XP或Ghost XP系统安装HDSC CDC驱动

易见错误一：
    “INF中的服务安装段落无效”。

解决方法：
    1、打开Windows目录下的安装记录文件setupapi.log，查看是否提示："C:\WINDOWS\system32\DRIVERS\usbser.sys" 不存在。
        #-035 正在处理服务添加/删除段 [DriverInstall.NT.Services]。
        #E339 添加服务: 服务 "usbser" 的二进制文件 "C:\WINDOWS\system32\DRIVERS\usbser.sys" 不存在。
        #E032 错误 0xe0000217: 这个 INF 中的服务安装段落无效。
        #E275 安装服务时出错。 错误 0xe0000217: 这个 INF 中的服务安装段落无效。
        #E122 设备安装失败。 错误 0xe0000217: 这个 INF 中的服务安装段落无效。
        #E154 类别安装程式失败。 错误 0xe0000217: 这个 INF 中的服务安装段落无效。

    2、若提示usbser.sys不存在，添加如下文件: 
        a、mdmcpq.inf_usbser.sys\mdmcpq.inf复制到c:\windows\inf；
        b、mdmcpq.inf_usbser.sys\usbser.sys复制到c:\windows\system32\drivers。

    3、卸载驱动，重新安装驱动。