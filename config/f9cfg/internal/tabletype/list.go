package tabletype

//@list 结构
type ListConfigBuilder struct {
	BaseConfigBuilder
}

func (c *ListConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {

	config = c.BuildBase(def)
	config.VarType = TypeList
	return
}
