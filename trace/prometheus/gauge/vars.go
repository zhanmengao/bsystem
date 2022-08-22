package gauge

import "github.com/prometheus/client_golang/prometheus"

var Online = prometheus.NewGaugeVec(
	prometheus.GaugeOpts{
		Name: "ta_fim_online",
		Help: "fim在线人数",
	}, []string{"midplat"})
