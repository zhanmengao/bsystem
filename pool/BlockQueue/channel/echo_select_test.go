package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"testing"
	"time"
)

func Test_Echo(t *testing.T) {
	listener, err := net.Listen("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}

	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn2(conn)
	}
}

func handleConn2(c net.Conn) {
	defer c.Close()
	var rBuf []byte = make([]byte, 16)
	rh := make(chan []byte, 10)
	//abort := make(chan struct{})
	go func() {
		for {
			tick := time.NewTicker(10 * time.Second)
			select {
			case s, ok := <-rh:
				{
					if ok {
						fmt.Printf("recv %s \n", s)
						tick.Stop()
					} else {
						return
					}
				}
			case <-tick.C:
				{
					fmt.Printf("%s client close\n", c.RemoteAddr())
					c.Close()
				}
			}
		}
	}()
	for {
		_, err := io.ReadFull(c, rBuf)
		if err != nil {
			log.Fatal(err)
			close(rh)
			return
		}
		rh <- rBuf
	}
}
