package tabletype

import (
	"errors"
	"fmt"
)

type MapConfigBuilder struct {
	BaseConfigBuilder
}

func (c *MapConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {
	if len(def.Params) < 1 {
		err = errors.New(fmt.Sprintf("Bad params: %s", def.Sheet))
		return
	}

	config = c.BuildBase(def)
	config.VarType = TypeMap
	config.Key = ToBigHump(def.Params[0])
	config.KeyType = def.FieldType[def.Params[0]]
	return
}
