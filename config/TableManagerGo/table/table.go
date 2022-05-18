package table
//make by robot . don't edit!!!!
var bossshow map[int32]*table.BossShow
func GetBossShowByID(id int32)*table.BossShow{
v,ok := bossshow[id]
if ok{
return v
}else{
return nil
}
}