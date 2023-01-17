package main

import (
	"log"
	pidstat "sysinfo/pidstat/gpidstat"
	"time"
)

func main() {
	for {
		time.Sleep(100 * time.Millisecond)
		log.Println(pidstat.CPUInfoByProc(320685))
	}
}
