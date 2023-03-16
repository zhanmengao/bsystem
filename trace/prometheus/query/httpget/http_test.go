package httpget

import "testing"

func TestQuery(t *testing.T) {
	res, err := QueryMetric("http://172.21.28.141:9090", "midplat_request_total{}")
	if err != nil {
		t.Fatalf(err.Error())
	}
	t.Log(res.Data)
}
