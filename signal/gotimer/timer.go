package main

import (
	"fmt"
	"time"
)

func main() {
	go runAfter()
	go runTicker()
	go runTick()
	select {}
}

func runAfter() {
	count := 0
	tm := time.After(time.Duration(count) * time.Second)
	for {
		select {
		case <-tm:
			count++
			fmt.Println("after ",count)
			tm = time.After(time.Duration(count) * time.Second)
		}
	}
}

func runTicker() {
	count := 0
	tm := time.NewTicker(time.Duration(5) * time.Second)
	for {
		select {
		case <-tm.C:
			count++
			fmt.Println("ticker ",count)
		}
	}
}

func runTick() {
	count := 0
	tm := time.Tick(time.Duration(5) * time.Second)
	for {
		select {
		case <-tm:
			count++
			fmt.Println("tick ",count)
		}
	}
}