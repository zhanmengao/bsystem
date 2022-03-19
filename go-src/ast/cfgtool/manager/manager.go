package forevernine_com_config

import (
	"sync"
	"sync/atomic"
)

var (
	metaLock  sync.RWMutex
	tableList = make(map[string]tableMetaInterface, 0) //表格基类指针map，包含load进来的数据
)

// RegisterTable 将表注册进去
func registerTable(tabName string, meta tableMetaInterface) {
	metaLock.Lock()
	defer metaLock.Unlock()
	tableList[tabName] = meta
}

// RegisterGetter 注册每个server生成ABTestUser的回调函数
func RegisterGetter(getInfoCaller func(interface{}) *ABTestUser) {
	getter[1] = getInfoCaller
}

// RequestTable 每个业务单独调用，声明自己需要的table
func RequestTable(tabName ...string) {
	getter[0] = defaultGetter
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

func RegisterSheetChange(sheetName string, cb TableChangedCB) {
	watchLock.Lock()
	defer watchLock.Unlock()
	if _, ok := onConfigChangeCallbacks[sheetName]; !ok {
		onConfigChangeCallbacks[sheetName] = make([]TableChangedCB, 0, 1)
	}
	onConfigChangeCallbacks[sheetName] = append(onConfigChangeCallbacks[sheetName], cb)
}
