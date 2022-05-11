package Struct

import pb "pthread/RoutinueIPC/proto"

type CKLockData struct {
	UID      int64
	LockTime int64
}

func (p *CKLockData) BuildFromProto(pbData *pb.CKLockedData) {
	p.UID = Str2Int(pbData.UID)
	p.LockTime = pbData.LockTime
}
