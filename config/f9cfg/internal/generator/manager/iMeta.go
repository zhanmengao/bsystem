package gconf

import "sync"

var (
	metaLock  sync.RWMutex
	tableList = make(map[string]iMeta, 0) //表格基类指针map，包含load进来的数据
)

type iMeta interface {
	getTable(abFilterName string) interface{}
}

// RegisterTable 将表注册进去
func getOrLoadTable(tabName string, meta iMeta) (tab iMeta) {
	metaLock.Lock()
	defer metaLock.Unlock()
	var exist bool
	if tab, exist = tableList[tabName]; exist {
	} else {
		tableList[tabName] = meta
		tab = meta
	}
	return
}
