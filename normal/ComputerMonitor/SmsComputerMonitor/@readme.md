# 简介 #
软件用于实时监控当前系统资源等情况，并调用接口，当资源被超额占用时，发送警报到个人手机；界面模拟Console的显示方式，信息缓冲大小由配置决定；可以定制监控的资源，包括：

- cpu使用率；
- 内存使用率；
- 磁盘使用率；
- 网络使用率；
- 进程线程数；

----------

# 配置说明 #

主要配置分为同程序路径下的三个文件：

1. SmsCpuMonitor.exe.config（主配置）
1. ProcessMoniterConfig.xml（进程监控配置）
1. NLog.config（日志记录配置）

----------
## 主配置使用说明 ##

主要**参数**有：

- CacheMaxCount 信息缓存数量，默认60条；
- InterTime 监控时间间隔，默认5s；
- SmsUrl 调用Sms发送接口地址；
- CpuLimitedValue CPU使用率限速 默认70%；
- PhoneList 发送信息手机列表，用|分割；
- ContinuityNoticeFrequency 连续错误发送频率，默认1次；
- DrivePercentUsedLimited 磁盘使用率限制 默认70%；
- MemoryPercentUsedLimited 内存使用率限制 默认80%；
- TotalNetSpeedKbpsLimited 网速使用限制 默认500Kbps；

----------
## 进程监控配置说明 ##

每个项目添加一个*item结点*：


- ProcessName 进程名词，注意非TaskMgr.exe中看到的名称；
- MaxThreadsCount 该进程限制最大线程数 默认50个；

----------

## 日志记录配置说明 ##

参考：[https://github.com/nlog/nlog/wiki/Tutorial](https://github.com/nlog/nlog/wiki/Tutorial)

----------

## 其他内容介绍 ##

EmEditor 

> 一款可以快速打开 大log文件的文本编辑器，用于打开日志文件；

CpuPress

> 自写的用于压测CPU使用率的工具；