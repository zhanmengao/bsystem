package main

import (
	"fmt"
	"github.com/robfig/cron/v3"
	"time"
)

func main() {
	c := cron.New()

	c.AddFunc("@every 1s", func() {
		fmt.Println("tick every 1 second run once")
	})
	c.Start()
	time.Sleep(time.Second * 10)
}
