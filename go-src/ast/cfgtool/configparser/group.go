package main

import (
	"errors"
	"fmt"
	"strings"
)

//@group 结构
type GroupConfigBuilder struct {
	BaseConfigBuilder
}

func (c *GroupConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig,
	err error) {
	if len(def.Params) < 1 {
		err = errors.New(fmt.Sprintf("Bad params: %s", def.Sheet))
		return
	}

	tmp := strings.Split(def.Params[0], ":")
	key := tmp[0]
	keyType := tmp[1]

	config = c.BuildBase(def)
	config.Key = ToBigHump(key)
	config.KeyType = keyType
	config.VarType = TypeGroup
	config.FieldName = c.groupName(key, def)
	return
}
