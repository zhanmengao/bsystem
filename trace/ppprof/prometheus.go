package main

import (
	"github.com/prometheus/client_golang/prometheus/promhttp"
	"net/http"
)

func initPrometheus() {
	http.Handle("/metrics", promhttp.Handler())
}
