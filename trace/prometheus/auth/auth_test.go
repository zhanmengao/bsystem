package auth

import (
	"context"
	"counter/histogram_conter"
	"counter/push_gateway"
	"fmt"
	"math/rand"
	"testing"
	"time"
)

func TestPush(t *testing.T) {
	push_gateway.InitPush(context.Background(), "172.21.28.141:9090", "lan:test", "testsrv", "lan")
	push_gateway.UseBasicAuth("1259411933", "c,73hrl~aF8QbtaqhBn0!Eru!wL")

	histogram_conter.InitRPCProm("lan")

	for {
		cmd := fmt.Sprintf("test_cmd_%d", rand.Int31n(10000))
		histogram_conter.ReportRequest("lan", cmd)
		histogram_conter.ReportDuration("lan", cmd, time.Duration(rand.Int31n(10000))*time.Millisecond)
		time.Sleep(time.Millisecond * 5)
	}
}
