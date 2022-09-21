package gconf

import "sync"

type tableMetaData struct {
	lock    sync.RWMutex
	tab     map[string]interface{}
	tabName string
}

func newMetaData(tabName string) *tableMetaData {
	meta := &tableMetaData{
		tabName: tabName,
		tab:     make(map[string]interface{}),
	}
	return meta
}

func (d *tableMetaData) getTable(filterName string) interface{} {
	d.lock.RLock()
	defer d.lock.RUnlock()
	if tab, exist := d.tab[filterName]; exist {
		return tab
	}
	return nil
}

func (d *tableMetaData) register(filterName string, tab interface{}) {
	d.lock.Lock()
	defer d.lock.Unlock()
	d.tab[filterName] = tab
}
