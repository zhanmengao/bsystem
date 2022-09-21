package gconf

import (
	"sync/atomic"
)

// RequestTable 每个业务单独调用，声明自己需要的table
func RequestTable(tabName ...string) {
	watchLock.RLock()
	defer watchLock.RUnlock()
	for _, v := range tabName {
		if wi, ok := watchInfo[v]; ok {
			if wi.watch != nil && atomic.CompareAndSwapInt32(&wi.isWatch, 0, 1) {
				wi.watch()
			}
		}
	}
}
