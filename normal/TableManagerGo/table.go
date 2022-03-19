package table

//make by robot . don't edit!!!!
var bossshow map[int32]*pb.BossShow

func GetBossShowByID(id int32) *pb.BossShow {
	v, ok := bossshow[id]
	if ok {
		return v
	} else {
		return nil
	}
}
