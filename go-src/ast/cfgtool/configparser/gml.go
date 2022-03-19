package main

import (
	"errors"
	"fmt"
	"strings"
)

// GroupMapListConfigBuilder @groupmaplist
type GroupMapListConfigBuilder struct {
	BaseConfigBuilder
}

func (c *GroupMapListConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {
	if len(def.Params) < 2 {
		err = errors.New(fmt.Sprintf("Bad params:%s", def.Sheet))
		return
	}
	config = &SheetConfig{}
	tmp := strings.Split(def.Params[0], ":")
	key := tmp[0]
	keyType := tmp[1]
	tmp1 := strings.Split(def.Params[1], ":")
	subKey := tmp1[0]
	subKeyType := tmp1[1]

	config = &SheetConfig{
		VarType:          TypeGroupMapList,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		Key:              ToBigHump(key),
		KeyType:          keyType,
		AryStructName:    c.aryName(def.Sheet),
		FuncName:         c.funcName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.CfgStruct),
		FieldName:        c.groupMapName(key, def),
		SubKey:           ToBigHump(subKey),
		SubKeyType:       ToHump(subKeyType),
		SubKeyLower:      ToHump(subKey),
	}
	config.FuncName = c.funcName(config.FieldName)
	if len(def.Params) >= 6 {
		config.Value, config.ValueType = def.Params[3], c.valueType(def.Params[4])
	} else {
		config.ValueType = c.valueType(PackagePath + config.ConfigStructName)
	}
	return
}
