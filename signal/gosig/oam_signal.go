// Package oam
// Copyright 2019 Forevernine
// Author yison@forevernine.com
// The software is NOT free software
//Any terms of copy or usage of the code should be under the permission of the author

package oam

import (
	"os"
	"os/signal"
	"syscall"
)

//DefaultSignalSet default signal set to catch
var DefaultSignalSet = []os.Signal{syscall.SIGTERM, syscall.SIGINT, syscall.SIGQUIT}

//SetSignalHandle set signal handle
func SetSignalHandle(sigHandle func(os.Signal), extSig ...os.Signal) (err error) {
	ch := make(chan os.Signal)
	tmpSig := make([]os.Signal, 0, len(DefaultSignalSet)+len(extSig))
	tmpSig = append(tmpSig, DefaultSignalSet...)
	tmpSig = append(tmpSig, extSig...)
	signal.Notify(ch, tmpSig...)
	go func() {
		for sig := range ch {
			sigHandle(sig)
		}
	}()
	return
}
