package query

import (
	"context"
	"fmt"
	"github.com/prometheus/client_golang/api"
	"github.com/prometheus/common/model"
	"testing"
	"time"
)

func TestQueryByCli(t *testing.T) {
	cli, err := api.NewClient(api.Config{
		Address:      "http://172.21.28.141:9090",
		RoundTripper: nil,
	})
	if err != nil {
		t.Fatalf(err.Error())
	}
	ctx := context.Background()
	pmql := NewAPI(cli)
	val, warn, err := pmql.Query(ctx, fmt.Sprintf("sum(rate(midplat_request_total{cmd=\"%s\",t_srv_name=\"%s\"}[5m]))by(cmd,t_srv_name)", "LoginV1", "fimsrv"), time.Now())
	if err != nil {
		t.Fatalf(err.Error())
	}
	t.Log(warn)
	vec := val.(model.Vector)
	for i := 0; i < vec.Len(); i++ {
		v := vec[i]
		t.Logf("val = %s \n", v.Value.String())
	}
}
