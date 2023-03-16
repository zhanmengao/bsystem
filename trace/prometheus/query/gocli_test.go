package query

import (
	"context"
	"github.com/prometheus/client_golang/api"
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
	val, warn, err := pmql.Query(ctx, "midplat_request_total{}", time.Now())
	if err != nil {
		t.Fatalf(err.Error())
	}
	t.Log(warn)
	t.Log(val)
	
}
