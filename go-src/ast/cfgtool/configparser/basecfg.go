package main

import (
	"strings"
)

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
	mn := "Table" + sheetName
	return mn
}

func (c *BaseConfigBuilder) valueType(vt string) string {
	// pb结构，用指针类型
	if strings.Contains(vt, PackagePath) {
		vt = "*" + vt
	}

	return vt
}

func (c *BaseConfigBuilder) aryType(vt string) string {
	if strings.Contains(vt, PackagePath) {
		vt = "*" + vt
	}
	return vt
}

func (c *BaseConfigBuilder) BuildBase(def *ConfigDefine) (config *SheetConfig) {
	config = &SheetConfig{
		Desc:             def.Desc,
		Sheet:            def.Sheet,
		SheetVar:         c.sheetVarName(def.Sheet),
		AryStructName:    c.aryName(def.Sheet),
		FuncName:         c.funcName(def.Sheet),
		ConfigStructName: c.configName(def.Sheet, def.CfgStruct),
	}
	config.ValueType = c.valueType(PackagePath + config.ConfigStructName)
	config.AryType = c.aryType(PackagePath + config.AryStructName)
	return
}
