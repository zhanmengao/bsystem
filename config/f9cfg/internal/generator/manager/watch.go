package gconf

import "sync"

type watchCB struct {
	watch   func()
	isWatch int32
}

var (
	watchLock sync.RWMutex
	watchInfo = make(map[string]*watchCB, 0) //watch的回调函数集，包含所有表格的函数
)

type TableChangedCB func(sheetName string)

// RegisterWatchCB 由pbconf调用，将自己的watch函数注册进去。业务禁止调用
func registerWatchCB(tabName string, cb func()) {
	watchLock.Lock()
	defer watchLock.Unlock()
	watchInfo[tabName] = &watchCB{
		watch:   cb,
		isWatch: 0,
	}
}
