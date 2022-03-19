package main

import (
	"errors"
	"fmt"
	"strings"
)

type MapConfigBuilder struct {
	BaseConfigBuilder
}

func (c *MapConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {
	if len(def.Params) < 1 {
		err = errors.New(fmt.Sprintf("Bad params: %s", def.Sheet))
		return
	}

	tmp := strings.Split(def.Params[0], ":")
	key := tmp[0]
	keyType := tmp[1]

	config = c.BuildBase(def)
	config.VarType = TypeMap
	config.Key = ToBigHump(key)
	config.KeyType = keyType
	config.FieldName = c.mapName(key, def)
	return
}
