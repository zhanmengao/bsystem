package Struct

import (
	"strconv"
	"sync"
	"time"
)

type UIDCache struct {
	lock sync.RWMutex
	m    map[int64]bool
}

func newUIDCache() *UIDCache {
	return &UIDCache{
		m: make(map[int64]bool),
	}
}

// IsContain 返回是否是在集合
func (n *UIDCache) IsContain(uid string) bool {
	n.lock.RLock()
	defer n.lock.RUnlock()
	_, ok := n.m[Str2Int(uid)]
	return ok
}

func (n *UIDCache) Add(uid string) {
	n.lock.Lock()
	defer n.lock.Unlock()
	n.m[Str2Int(uid)] = true
}

func (n *UIDCache) Del(uid string) {
	n.lock.Lock()
	defer n.lock.Unlock()
	delete(n.m, Str2Int(uid))
}

func (n *UIDCache) GetSlice() []int64 {
	l := make([]int64, 0, len(n.m))
	n.lock.RLock()
	defer n.lock.RUnlock()
	for k, _ := range n.m {
		l = append(l, k)
	}
	return l
}

func IsUserActive(lastLogin int64) bool {
	login := time.Unix(lastLogin, 0)
	limit := time.Unix(time.Now().Unix()-6*24*60*60, 0)
	return login.After(limit)
}

func Str2Int(uid string) int64 {
	i, _ := strconv.ParseInt(uid, 10, 64)
	return i
}
func Int2Str(uid int64) string {
	s := strconv.FormatInt(uid, 10)
	return s
}
