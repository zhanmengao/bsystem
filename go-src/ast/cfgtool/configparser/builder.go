package main

const (
	Mark        = "@"
	PackagePath = "pb."
)

var ConfigBuilders map[string]IConfigBuilder

type SheetConfig struct {
	Desc     string // 注释
	VarType  string // 变量类型 struct map list
	Sheet    string // 表
	SheetVar string // 表变量名

	Key         string // map 的 key
	KeyType     string
	SubKeyType  string
	SubKey      string
	SubKeyLower string

	AryType   string
	Value     string
	ValueType string

	AryStructName    string // ary结构体, (不包括包名)
	ConfigStructName string // 配置结构体名字, (不包含包名)
	FieldName        string // 结构体field
	FuncName         string // 函数名后缀
	//StructType string // 自定义结构类型
	//ModuleName   string // 功能模块名
	ComplexSheet *SheetConfig
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
