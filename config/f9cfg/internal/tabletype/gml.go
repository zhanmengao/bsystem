package tabletype

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

	config = &SheetConfig{
		VarType:          TypeGroupMapList,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		Key:              ToBigHump(key),
		KeyType:          keyType,
		AryStructName:    c.aryName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.StructType),
	}
	if len(def.Params) >= 6 {
		config.Value, config.ValueType = def.Params[3], c.valueType(def.Params[4])
	} else {
		config.ValueType = c.valueType(PackagePath + config.ConfigStructName)
	}
	panic("Not support")
}
