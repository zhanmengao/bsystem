package tabletype

import (
	"errors"
	"fmt"
)

// GroupMapConfigBuilder @groupmap
type GroupMapConfigBuilder struct {
	BaseConfigBuilder
}

//Build Platform,Channel
func (c *GroupMapConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {
	if len(def.Params) < 2 {
		err = errors.New(fmt.Sprintf("Bad params:%s", def.Sheet))
		return
	}
	config = c.BuildBase(def)

	config.Key = ToBigHump(def.Params[0])
	config.KeyType = def.FieldType[def.Params[0]]
	config.SubKey = ToBigHump(def.Params[1])
	config.SubKeyType = def.FieldType[def.Params[1]]

	config.ValueType = c.valueType(PackagePath + config.ConfigStructName)
	config.VarType = TypeGroupMap
	return
}
