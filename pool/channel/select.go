package main

import (
	"fmt"
	"log"
	"os"
	"time"
)

func main(){
	abort := make(chan struct{})
	go func() {
		recv := make([]byte, 2)
		n, err := os.Stdin.Read(recv)
		if err != nil {
			log.Fatalln(err)
		}
		fmt.Printf("recv %d : %s ", n, string(recv))
		abort <- struct{}{}
	}()

	tick := time.Tick(1 * time.Second)
	for countdown := 10; countdown > 0; countdown-- {
		fmt.Println(countdown)
		select {
		case t := <-tick:
			fmt.Printf("tick %v \n", t)
		case <-abort:
			fmt.Println("Launch aborted!")
			return
		}
	}
	launch()
}

func launch() {
	fmt.Println("Lift off!")
}
