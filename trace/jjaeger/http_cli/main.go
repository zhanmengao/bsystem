package main

import (
	"bufio"
	"os"
	"ttrace/jjaeger/ftracer"
)

func main() {
	tracer, closer, _ := ftracer.CreateTracer("UserinfoService")
	// 创建第一个 span A
	parentSpan := tracer.StartSpan("A")
	// 调用其它服务
	GetUserInfo(tracer, parentSpan)
	// 结束 A
	parentSpan.Finish()
	// 结束当前 tracer
	closer.Close()

	reader := bufio.NewReader(os.Stdin)
	_, _ = reader.ReadByte()
}
