package routinue

import (
	"io"
	"log"
	"net"
	"os"
	"testing"
)

func Test_RecvServer(t *testing.T){
	listener,err :=net.Listen("tcp","localhost:8000")
	if err!=nil{
		log.Fatal(err)
	}
	for{
		conn,err:=listener.Accept()
		if err!=nil{
			log.Print(err)
			continue
		}
		go handleConnect(conn)
	}
}

func handleConnect(c net.Conn){
	io.Copy(os.Stdout,c)
	defer c.Close()
}