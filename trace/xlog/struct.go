package xlog

import (
	"fmt"
	"os"
)

const Version = "0.1.1"

// 禁用日志输出, 设置日志等级OFF
const (
	TRACE Level = iota - 2
	DEBUG
	INFO
	NOTICE
	WARN
	ERROR
	FATAL
	OFF
)

// 日志等级
//type Level uint32
type Level int8

// 日志所属基本信息, 告警、统计定制化配置
//type Metric struct {
//	Api       string `json:"api,omitempty"`       // 接口标识
//	ReqId     string `json:"reqId,omitempty"`     // 请求ID, 标识请求链, 无则自动生成, 调用多个服务, 应保持一致
//	Uid       string `json:"uid,omitempty"`       // 用户唯一标识
//	Ip        string `json:"ip,omitempty"`        // 客户端IP
//	Lan       string `json:"lan,omitempty"`       // 客户端语言
//	Time      string `json:"time,omitempty"`      // 客户端请求时间戳
//	Device    string `json:"device,omitempty"`    // 设备类型
//	Channel   string `json:"channel,omitempty"`   // 渠道
//	Platform  string `json:"platform,omitempty"`  // 平台名称
//	Reversion string `json:"reversion,omitempty"` // 客户端热更新版本
//	Version   string `json:"version,omitempty"`   // 客户端版本
//	Country   string `json:"country,omitempty"`   // 客户端国家
//	Timezone  string `json:"timezone,omitempty"`  // 客户端时区
//	App       string `json:"app,omitempty"`
//}

func (level Level) String() string {
	switch level {
	case TRACE:
		return "TRACE"
	case DEBUG:
		return "DEBUG"
	case INFO:
		return "INFO"
	case NOTICE:
		return "NOTICE"
	case WARN:
		return "WARN"
	case ERROR:
		return "ERROR"
	case FATAL:
		return "FATAL"
	default:
		fmt.Fprintf(os.Stderr, "[logsdk] level unknown: %d\n", level)
		return "UNKNOWN"
	}
}
