package main

import (
	"io"
	"log"
	"net"
	"os"
	"fmt"
)

func main() {
	conn, err := net.Dial("tcp", "localhost:8000")
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) 						//ignoring errors
		log.Println("done")
		done <- struct{}{} 								//send
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done 												//recv
	fmt.Printf("main routinue recv end \n")
}


func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
