package proc

import (
	pb "pthread/RoutinueIPC/proto"
)

func handleStealEvent(req *pb.UrvStealEvent) error {
	return nil
}

func handleLockEvent(req *pb.UrvLockEvent) error {
	return nil
}

func handleStealPoolReq(req *pb.UrvStealPoolReq, srvName, from string) error {
	//get friend list
	return nil
	//send
	//dAtA,_ := rsp.Marshal()
}
