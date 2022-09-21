package tabletype

//@struct 结构
type StructConfigBuilder struct {
	BaseConfigBuilder
}

func (c *StructConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig,
	err error) {
	config = c.BuildBase(def)
	config.VarType = TypeStruct
	return
}
