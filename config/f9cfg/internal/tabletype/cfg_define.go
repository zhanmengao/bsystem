package tabletype

import (
	"config/f9cfg/internal/meta"
	"errors"
	"go/ast"
	"strings"
)

type ConfigDefine struct {
	Sheet      string
	CfgType    string
	AryName    string
	StructType string
	FieldType  map[string]string //field,type

	Params []string
}

// NewConfigDefine @cfg:map|Platform
func NewConfigDefine(aryName, comment, pbPkg string, fieldType *ast.TypeSpec) *ConfigDefine {
	def := &ConfigDefine{
		Sheet:     strings.Replace(aryName, "ConfigAry", "", 1),
		FieldType: make(map[string]string, 5),
		AryName:   aryName,
	}

	paramsList := strings.Split(comment, Separator)
	paramsList[0] = strings.Trim(paramsList[0], "// ")
	cfgType := strings.Split(paramsList[0], ":")[1]

	// 定义类型
	def.CfgType = cfgType
	def.StructType = fieldType.Name.Name

	def.Params = paramsList[1:]
	for _, v := range fieldType.Type.(*ast.StructType).Fields.List {
		t, n := meta.GetFieldName(v, pbPkg)
		def.FieldType[n] = t
	}
	return def
}

func (c *ConfigDefine) ParseComment() (sheetConfig *SheetConfig, err error) {
	// 生成不同类型的配置
	if builder, ok := ConfigBuilders[c.CfgType]; ok {
		sheetConfig, err = builder.Build(c)
		if err != nil {
			return
		}
	} else {
		err = errors.New("not allow")
	}
	return
}
