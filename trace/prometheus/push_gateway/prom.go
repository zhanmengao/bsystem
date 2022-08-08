package push_gateway

import (
	"context"
	"github.com/prometheus/client_golang/prometheus"
	"github.com/prometheus/client_golang/prometheus/push"
	"log"
	"net"
	"net/url"
	"os"
	"strings"
	"time"
)

var (
	closeCh      = make(chan struct{})
	gwAddr       string
	hostName     string
	instanceName string
	jobName      string
	serverName   string
	srvPlatform  string
)

var collectors []prometheus.Collector

// AddCollector 添加监控指标
func AddCollector(cs ...prometheus.Collector) {
	collectors = append(collectors, cs...)
}

// InitPush Prometheus启动主动上报到pushgateway服务
//  gatewayAddr 上报地址
//  instance    实例名称
//  srvName     服务名
//  platform    平台名
func InitPush(ctx context.Context, gatewayAddr, instance, srvName, platform string) {
	instanceName = instance
	hostName, _ = os.Hostname()
	jobName = srvName + "_push"
	serverName = srvName
	srvPlatform = platform

	SetGateway(gatewayAddr)
	go pushTick(ctx)
}

// SetGateway 修改上报的网关地址
//  addr 上报地址
func SetGateway(addr string) {
	if addr == "" {
		log.Println("prometheus gatewaypush is not enabled")
		return
	}
	destUrl := ""
	if strings.Contains(addr, "://") {
		destUrl = addr
	} else {
		destUrl = "http://" + addr
	}
	u, err := url.Parse(destUrl)
	if err != nil {
		return
	}
	host := u.Host
	if !strings.Contains(u.Host, ":") {
		host += ":80"
	}
	conn, err := net.DialTimeout("tcp", host, time.Second)
	if err != nil {
		return
	}
	conn.Close()
	gwAddr = addr
}

func pushTick(ctx context.Context) {
	tk := time.NewTicker(10 * time.Second)
	for {
		select {
		case <-tk.C:
			if gwAddr == "" {
				return
			}
			cli := getPushCli()
			cli.Push()
		case <-closeCh:
			return
		}
	}
}

func getPushCli() *push.Pusher {
	cli := push.New(gwAddr, jobName)
	for _, c := range collectors {
		cli.Collector(c)
	}
	cli.Grouping("instance", instanceName).
		Grouping("cvm_instance_name", hostName).
		Grouping("srv_name", serverName).
		Grouping("t_platform", srvPlatform)
	return cli
}

// DeInit 删除自己的push地址指标
func DeInit() {
	close(closeCh)
	cli := getPushCli()
	cli.Delete()
}
