package proc

import (
	pb "pthread/RoutinueIPC/proto"
)

func RegisterCMD(){

}
func HandleSrvPacket(srvName, from string, data []byte) (err error) {
	req := pb.IPCMessage{}

	err = req.Unmarshal(data)
	if err != nil {
		return err
	}
	switch req.Head.PakId {
	case pb.IPCMessageLoginEvent:
		{
			pak := &pb.UrvLoginEvent{}
			err = pak.Unmarshal(req.Data)
			if err != nil {
				return
			}
			err = handleLoginEvent(pak)
		}
	case pb.IPCMessageMoneyEvent:
		{
			pak := &pb.UrvMoneyEvent{}
			err = pak.Unmarshal(req.Data)
			if err != nil {
				return
			}
			err = handleMoneyEvent(pak)
		}
	case pb.IPCMessageLockEvent:
		{
			pak := &pb.UrvLockEvent{}
			err = pak.Unmarshal(req.Data)
			if err != nil {
				return
			}
			err = handleLockEvent(pak)
		}
	case pb.IPCMessageStealEvent:
		{
			pak := &pb.UrvStealEvent{}
			err = pak.Unmarshal(req.Data)
			if err != nil {
				return
			}
			err = handleStealEvent(pak)
		}
	case pb.IPCMessageStealPoolReq:
		{
			pak := &pb.UrvStealPoolReq{}
			err = pak.Unmarshal(req.Data)
			if err != nil {
				return
			}
			err = handleStealPoolReq(pak, srvName, from)
		}
	}
	return err
}

