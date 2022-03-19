package forevernine_com_config

type tableMapMeta struct {
	*tableMetaData
	maxID [tabArrayLen]interface{}
}

func (tmm *tableMapMeta) getMaxID(ctx interface{}) interface{} {
	idx := getABIndex(tmm.tabName, ctx)
	return tmm.maxID[idx]
}

func (tmm *tableMapMeta) getDefaultMaxID() interface{} {
	return tmm.maxID[0]
}

func newTableMapMeta(tabName string, tab interface{}, maxID interface{}) *tableMapMeta {
	meta := &tableMapMeta{
		tableMetaData: newMetaData(tabName, tab),
	}
	meta.maxID[0] = maxID
	registerTable(tabName, meta)
	return meta
}
