package gauge

import (
	"counter/push_gateway"
	"github.com/prometheus/client_golang/prometheus"
	"time"
)

func init() {
	prometheus.MustRegister(Online)

	push_gateway.AddCollector(Online)
}

func tick() {
	for {
		time.Sleep(time.Duration(1) * time.Minute)
		userTotal := make(map[string]int64)
		Online.Reset()
		for platform, count := range userTotal {
			Online.With(prometheus.Labels{
				"midplat": platform,
			}).Set(float64(count))
		}
	}
}
