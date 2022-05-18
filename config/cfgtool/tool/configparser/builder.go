package configparser

import (
	"errors"
	"fmt"
	"strings"
)

const (
	Mark        = "@"
	PackagePath = "pb."
)

var ConfigBuilders map[string]IConfigBuilder

type SheetConfig struct {
	BuildObjName   string
	BuildClassName string

	Desc     string // 注释
	VarType  string // 变量类型 struct map list
	Sheet    string // 表
	SheetVar string // 表变量名

	Key         string // map 的 key
	KeyType     string
	KeyLower    string // map 的 key 的小写
	SubKeyType  string
	SubKey      string
	SubKeyLower string

	Value     string
	ValueType string

	AryStructName    string // ary结构体, (不包括包名)
	ConfigStructName string // 配置结构体名字, (不包含包名)
	FieldName        string // 结构体field
	FuncName         string // 函数名后缀
	//StructType string // 自定义结构类型
	//ModuleName   string // 功能模块名
	ComplexSheet *SheetConfig

	AbTest   string
	DeepCopy bool
}

type ConfigDefine struct {
	Sheet     string
	T         string
	ObjName   string
	CfgStruct string
	FieldName string
	Desc      string

	Params []string
}

func init() {
	ConfigBuilders = make(map[string]IConfigBuilder)
	ConfigBuilders["map"] = &MapConfigBuilder{}
	ConfigBuilders["list"] = &ListConfigBuilder{}
	ConfigBuilders["struct"] = &StructConfigBuilder{}
	ConfigBuilders["group"] = &GroupConfigBuilder{}
	ConfigBuilders["groupmap"] = &GroupMapConfigBuilder{}
	ConfigBuilders["groupmaplist"] = &GroupMapListConfigBuilder{}
}

type IConfigBuilder interface {
	Build(def *ConfigDefine) (config *SheetConfig, err error)
}

type BaseConfigBuilder struct {
	Builder  interface{}
	codeTmpl string
}

//func (c *BaseConfigBuilder) tableName(sheetVarName string) string {
//	return sheetVarName
//}

func (c *BaseConfigBuilder) funcName(sheetName string) string {
	return ToBigHump(sheetName)
}

func (c *BaseConfigBuilder) configName(sheetName, structName string) string {
	if structName != "" {
		return structName
	}
	return sheetName + "Config"
}

func (c *BaseConfigBuilder) customName(header string) (name string, ok bool) {
	if strings.Contains(header, ":") {
		headerParams := strings.Split(header, ":")
		return headerParams[1], true
	}
	return
}

func (c *BaseConfigBuilder) aryName(sheetName string) string {
	return sheetName + "ConfigAry"
}

func (c *BaseConfigBuilder) varName(def *ConfigDefine) string {
	if def.FieldName != "" {
		return ToHump(def.FieldName)
	}

	return ToHump(def.Sheet + "Struct")
}

func (c *BaseConfigBuilder) mapName(keyName string, def *ConfigDefine) string {
	if def.FieldName != "" {
		return ToHump(def.FieldName)
	}
	mn := ToHump(def.Sheet + ToBigHump(keyName) + "Map")
	return mn
}

func (c *BaseConfigBuilder) listName(def *ConfigDefine) string {
	if def.FieldName != "" {
		return ToHump(def.FieldName)
	}

	//if cn, ok := c.customName(header); ok {
	//	return cn
	//}

	mn := ToHump(def.Sheet + "List")
	return mn
}

func (c *BaseConfigBuilder) groupName(keyName string, def *ConfigDefine) string {
	if def.FieldName != "" {
		return ToHump(def.FieldName)
	}

	mn := ToHump(def.Sheet + ToBigHump(keyName) + "ConfigGroup")
	return mn
}

func (c *BaseConfigBuilder) groupMapName(keyName string, def *ConfigDefine) string {
	if def.FieldName != "" {
		return ToHump(def.FieldName)
	}

	mn := ToHump(def.Sheet + ToBigHump(keyName) + "ConfigGroupMap")
	return mn
}

func (c *BaseConfigBuilder) sheetVarName(sheetName string) string {
	mn := "sheetVarName" + sheetName
	return mn
}

func (c *BaseConfigBuilder) valueType(vt string) string {
	// pb结构，用指针类型
	if strings.Contains(vt, PackagePath) {
		vt = "*" + vt
	}

	return vt
}

//@struct 结构
type StructConfigBuilder struct {
	BaseConfigBuilder
}

func (c *StructConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig,
	err error) {
	config = &SheetConfig{
		Desc:             def.Desc,
		VarType:          TypeStruct,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		AryStructName:    c.aryName(def.Sheet),
		FuncName:         c.funcName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.CfgStruct),
		FieldName:        c.varName(def),
		//ModuleName: params[2],
	}
	config.FuncName = c.funcName(config.FieldName)

	return
}

//@list 结构
type ListConfigBuilder struct {
	BaseConfigBuilder
}

func (c *ListConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig,
	err error) {
	config = &SheetConfig{
		Desc:             def.Desc,
		VarType:          TypeList,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		FieldName:        c.listName(def),
		AryStructName:    c.aryName(def.Sheet),
		FuncName:         c.funcName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.CfgStruct),
		//ModuleName: params[2],
	}
	config.FuncName = c.funcName(config.FieldName)

	if len(def.Params) >= 5 {
		config.Value, config.ValueType = def.Params[1], c.valueType(def.Params[2])
	} else {
		valueTypeName := config.ConfigStructName
		if len(def.Params) > 2 && def.Params[1] != "" {
			valueTypeName = def.Params[1]
		}
		config.ValueType = c.valueType(PackagePath + valueTypeName)
	}

	return
}

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
	config = &SheetConfig{
		Desc:             def.Desc,
		VarType:          TypeMap,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		Key:              ToBigHump(key),
		KeyLower:         ToHump(key),
		KeyType:          keyType,
		FieldName:        c.mapName(key, def),
		AryStructName:    c.aryName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.CfgStruct),
		//ModuleName: params[4],
	}
	config.FuncName = c.funcName(config.FieldName)

	valueTypeName := config.ConfigStructName
	if len(def.Params) > 4 && def.Params[3] != "" {
		valueTypeName = def.Params[3]
	}
	config.ValueType = c.valueType(PackagePath + valueTypeName)
	return
}

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

	config = &SheetConfig{
		VarType:          TypeGroup,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		Key:              ToBigHump(key),
		KeyLower:         ToHump(key),
		KeyType:          keyType,
		AryStructName:    c.aryName(def.Sheet),
		FuncName:         c.funcName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.CfgStruct),
		FieldName:        c.groupName(key, def),
	}
	config.FuncName = c.funcName(config.FieldName)

	if len(def.Params) >= 5 {
		config.Value, config.ValueType = def.Params[3], c.valueType(def.Params[4])
	} else {
		config.ValueType = c.valueType(PackagePath + config.ConfigStructName)
	}

	return
}

// GroupMapConfigBuilder @groupmap
type GroupMapConfigBuilder struct {
	BaseConfigBuilder
}

func (c *GroupMapConfigBuilder) Build(def *ConfigDefine) (config *SheetConfig, err error) {
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
		VarType:          TypeGroupMap,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		Key:              ToBigHump(key),
		KeyLower:         ToHump(key),
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
		KeyLower:         ToHump(key),
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
