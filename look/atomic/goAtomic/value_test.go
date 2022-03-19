package goAtomic

import (
	"sync/atomic"
	"testing"
	"time"
)

var config atomic.Value

func loadConfig() map[string]string {
	return make(map[string]string) // 从数据库或者文件系统中读取配置信息，然后以map的形式存放在内存里
}

func requests() chan int {
	return make(chan int) // 将从外界中接受到的请求放入到channel里
}

func TickLoad() {
	// 每10秒钟定时的拉取最新的配置信息，并且更新到config变量里
	for {
		time.Sleep(10 * time.Second)
		// 对应于赋值操作 config = loadConfig()
		config.Store(loadConfig())
	}
}

func worker() {
	for r := range requests() {
		// 对应于取值操作 c := config
		// 由于Load()返回的是一个interface{}类型，所以我们要先强制转换一下
		c := config.Load().(map[string]string)
		// 这里是根据配置信息处理请求的逻辑...
		_, _ = r, c
	}
}

func Test_Atomic(t *testing.T) {
	// config变量用来存放该服务的配置信息
	// 初始化时从别的地方加载配置文件，并存到config变量里
	config.Store(loadConfig())

	go TickLoad()

	// 创建工作线程，每个工作线程都会根据它所读取到的最新的配置信息来处理请求
	for i := 0; i < 10; i++ {
		go worker()
	}
	select {}
}
