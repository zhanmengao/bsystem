package proc

import (
	"fmt"
	"log"
	pb "pthread/RoutinueIPC/proto"
	"testing"
)

func TestUrv(t *testing.T) {
	req := pb.IPCMessage{}
	req.Head = new(pb.IPCHeader)
	req.Head.PakId = pb.IPCMessageLockEvent

	ba, err := req.Marshal()
	if err != nil {
		log.Fatalln(err)
	}
	req.Unmarshal(ba)

	fmt.Printf("packet = %d \n", req.Head.PakId)
}
