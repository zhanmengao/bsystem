package gconf

type tableMapMeta struct {
	*tableMetaData
	maxID map[string]interface{}
}

func newTableMapMeta(tabName string) *tableMapMeta {
	meta := &tableMapMeta{
		tableMetaData: newMetaData(tabName),
		maxID:         make(map[string]interface{}),
	}
	return meta
}

func (tmm *tableMapMeta) getMaxID(abFilter string) interface{} {
	tmm.lock.RLock()
	defer tmm.lock.RUnlock()
	if max, exist := tmm.maxID[abFilter]; exist {
		return max
	}
	return nil
}

func (tmm *tableMapMeta) register(filterName string, tab interface{}, maxID interface{}) {
	tmm.lock.Lock()
	defer tmm.lock.Unlock()
	tmm.tab[filterName] = tab
	tmm.maxID[filterName] = maxID
}
