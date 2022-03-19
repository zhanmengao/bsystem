package Struct

import (
	pb "pthread/RoutinueIPC/proto"
	_ "sort"
	"time"
)

// UserCache 一个内存级的结构
type UserCache struct {
	uid           int64          //我的UID
	money         int64          //我有多少钱
	lastLoginTime int64          //我的上次登录时间
	stealData     StealDailyData //偷记录
	lockedRecord  []CKLockData   //被锁定记录
}

func NewUserCache(uid string) *UserCache {
	return &UserCache{
		uid: Str2Int(uid),
	}
}

func (cache *UserCache) BuildStealPoolFriend(info *pb.UrvStealPoolRspFriend) {
	info.Money = cache.money
	info.UID = Int2Str(cache.uid)
}
func (cache *UserCache) IsActive() bool {
	return IsUserActive(cache.lastLoginTime)
}

func (cache *UserCache) GetKey() string {
	return Int2Str(cache.uid)
}


func (cache *UserCache) GetUID() string {
	return cache.GetKey()
}

// CopyFrom 更新的时候调用
func (cache *UserCache) CopyFrom(uc *UserCache) {
	cache.money = uc.money
	cache.lastLoginTime = uc.lastLoginTime
}

// CheckReset 检查是否跨天，跨天则重置当天数据
func (cache *UserCache) CheckReset() {
	//每日重置检查（惰性重置）
	now := time.Now()
	lastModTm := time.Unix(cache.stealData.LastModTime, 0)
	//跨天，重置
	if lastModTm.Day() != now.Day() {
		cache.stealData.Reset()
	}
	//锁定超时检查
	count := 0
	for _, v := range cache.lockedRecord {
		if v.LockTime < now.Unix() {
			count++
		}
	}
	//如果有超时数据，这里才会重新分配内存
	if count > 0 {
		lockData := make([]CKLockData, 0, len(cache.lockedRecord)-count)
		for _, v := range cache.lockedRecord {
			if v.LockTime > now.Unix() {
				lockData = append(lockData, v)
			}
		}
		cache.lockedRecord = lockData
	}
}

func (cache *UserCache) GetLastLogin() int64 {
	return cache.lastLoginTime
}
func (cache *UserCache) SetLastLogin(tm int64) {
	cache.lastLoginTime = tm
}
func (cache *UserCache) GetLastLoginAgo() int {
	ago := time.Unix(time.Now().Unix()-time.Unix(cache.lastLoginTime, 0).Unix(), 0)
	return ago.Day()
}

func (cache *UserCache) GetLastSteal() string {
	return Int2Str(cache.stealData.LastStealUID)
}
func (cache *UserCache) GetMoney() int64 {
	return cache.money
}
func (cache *UserCache) SetMoney(money int64) {
	cache.money = money
}
func (cache *UserCache) IncMoney(inc int64) {
	cache.money += inc
}

// IsCanSteal 检查是否可偷
func (cache *UserCache) IsCanSteal(srcUid string, srcLastSteal string) bool {
	srcUid64 := Str2Int(srcUid)
	cache.CheckReset()
	//todo 配置
	if len(cache.stealData.StolenRecord) > 3 {
		return false
	} else if len(cache.lockedRecord) > 3 { //todo 配置
		return false
	} else if cache.money < 30000 {
		return false
	} else { //todo 配置
		//src是否偷了我{3}次
		count := 0
		for _, v := range cache.stealData.StolenRecord {
			if v.UID == srcUid64 {
				count++
			}
		}
		//todo 配置
		if count > 2 {
			return false
		}
	}
	//是否上次刚偷了我
	if srcLastSteal == cache.GetUID() {
		return false
	}
	//上线时间检查
	if cache.GetLastLoginAgo() >= 3 && cache.GetLastLoginAgo() != 5 {
		return false
	}
	return true
}

// AddStolenRecord 增加被偷记录
func (cache *UserCache) AddStolenRecord(stolen *pb.StealRecord) {
	var sr StealRecord
	sr.BuildFromProto(stolen)
	cache.stealData.StolenRecord = append(cache.stealData.StolenRecord, sr)
}

// AddLocked 增加被锁定记录
func (cache *UserCache) AddLocked(locked *pb.CKLockedData) {
	var cl CKLockData
	cl.BuildFromProto(locked)
	cache.lockedRecord = append(cache.lockedRecord, cl)
}

func (cache *UserCache) SetLastSteal(uid string) {
	cache.stealData.LastStealUID = Str2Int(uid)
}

// CacheVector 排序类
type CacheVector struct {
	arr []*UserCache
	//提前构造好时间，避免排序开销
	fiveDayAgo    time.Time
	userLastLogin []time.Time
}

func NewUserCacheVector(arr []*UserCache) *CacheVector {
	vector := &CacheVector{
		arr: arr,
	}
	//todo 配置
	vector.fiveDayAgo = time.Unix(time.Now().Unix()-5*24*60*60, 0)
	vector.userLastLogin = make([]time.Time, len(arr), len(arr))
	for i, v := range arr {
		vector.userLastLogin[i] = time.Unix(v.lastLoginTime, 0)
	}
	return vector
}

func (u *CacheVector) Len() int {
	return len(u.arr)
}
func (u *CacheVector) Swap(i, j int) {
	tmp := u.arr[i]
	u.arr[i] = u.arr[j]
	u.arr[j] = tmp
}
func (u *CacheVector) Less(i, j int) bool {
	//如果是正好第五天未登录，则优先
	if (u.userLastLogin[i].Day() == u.fiveDayAgo.Day()) &&
		(u.userLastLogin[j].Day() != u.fiveDayAgo.Day()) {
		return true
	} else if (u.userLastLogin[i].Day() != u.fiveDayAgo.Day()) &&
		(u.userLastLogin[j].Day() == u.fiveDayAgo.Day()) {
		return false
	} else {
		//否则 比较金钱
		return u.arr[i].money > u.arr[j].money
	}
}
