package main

import "net/http/pprof"

func initPprof() {
	_ = pprof.Trace
	return
}
