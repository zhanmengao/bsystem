package tabletype

import (
	"errors"
	"fmt"
)

//@group 结构
type GroupConfigBuilder struct {
	BaseConfigBuilder
}

//Build Platform
func (c *GroupConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {
	if len(def.Params) < 1 {
		err = errors.New(fmt.Sprintf("Bad params: %s", def.Sheet))
		return
	}

	config = c.BuildBase(def)
	config.Key = ToBigHump(def.Params[0])
	config.KeyType = def.FieldType[def.Params[0]]
	config.VarType = TypeGroup
	return
}
