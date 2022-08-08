package histogram_conter

import (
	"counter/push_gateway"
	"fmt"
	"github.com/prometheus/client_golang/prometheus"
	"sync"
	"time"
)

const (
	labelCMD            = "cmd"
	labelCallerPlatform = "platform"
)

var once sync.Once
var durations *prometheus.HistogramVec
var reqCount *prometheus.CounterVec

func InitRPCProm(projectName string) {
	once.Do(func() {
		durations = prometheus.NewHistogramVec(
			prometheus.HistogramOpts{
				Name:    fmt.Sprintf("%s_request_duration_ms", projectName),
				Help:    "接口耗时",
				Buckets: []float64{1, 100, 300, 500, 1000, 2000, 3000, 5000, 8000},
			},
			[]string{labelCallerPlatform, labelCMD},
		)
		reqCount = prometheus.NewCounterVec(
			prometheus.CounterOpts{
				Name: fmt.Sprintf("%s_request_total", projectName),
				Help: "服务总请求数",
			},
			[]string{labelCallerPlatform, labelCMD},
		)
		prometheus.MustRegister(durations, reqCount)
		push_gateway.AddCollector(durations, reqCount)
	})
}

// ReportDuration 上报接口延迟时间。fgrpc,fhttp,frpc会自动调用
// platform: 调用方平台
// cmd : 调用的接口
// use : 延迟(ns)
func ReportDuration(platform, cmd string, use time.Duration) {
	ms := float64(use.Milliseconds())
	durations.With(prometheus.Labels{
		labelCallerPlatform: platform,
		labelCMD:            cmd,
	}).Observe(ms)
}

// ReportRequest 统计QPS等。fgrpc,fhttp,frpc会自动调用
// platform 调用方的平台
// cmd 访问的借口或者URL
func ReportRequest(platform string, cmd string) {
	reqCount.With(prometheus.Labels{
		labelCallerPlatform: platform,
		labelCMD:            cmd,
	}).Inc()
}
