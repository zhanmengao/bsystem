package forevernine_com_config

type tableMetaInterface interface {
	getTable(ctx interface{}) interface{}
	getDefaultTable() interface{}
}

type tableMetaData struct {
	AbTest
	tabName string
}

func (d *tableMetaData) getTable(ctx interface{}) interface{} {
	return d.AbTest.getTable(d.tabName, ctx)
}

func newMetaData(tabName string, tab interface{}) *tableMetaData {
	meta := registerMetaData(tabName, tab)
	registerTable(tabName, meta)
	return meta
}
func registerMetaData(tabName string, tab interface{}) *tableMetaData {
	a := &tableMetaData{
		tabName: tabName,
	}
	a.register(tab)
	return a
}
