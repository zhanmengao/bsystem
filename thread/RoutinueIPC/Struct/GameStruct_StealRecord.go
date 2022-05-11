package Struct

import (
	pb "pthread/RoutinueIPC/proto"
	"time"
)

type StealRecord struct {
	UID       int64
	Money     int32
	StealTime int64
}

type StealDailyData struct {
	StolenRecord []StealRecord
	LastStealUID    int64
	LastModTime  int64
}

func (p *StealRecord) BuildFromProto(pbData *pb.StealRecord) {
	p.StealTime = pbData.StealTime
	p.UID = Str2Int(pbData.UID)
}

func (p *StealDailyData) Reset() {
	p.StolenRecord = make([]StealRecord, 0)
	p.LastStealUID = 0
	p.LastModTime = time.Now().Unix()
}
