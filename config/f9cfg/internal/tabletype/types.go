package tabletype

type SheetConfig struct {
	VarType  string // 变量类型 struct map list
	Sheet    string // 表
	SheetVar string // 表变量名

	Key        string // map 的 key
	KeyType    string
	SubKey     string
	SubKeyType string

	AryType   string
	Value     string
	ValueType string

	AryStructName    string // ary结构体, (不包括包名)
	ConfigStructName string // 配置结构体名字, (不包含包名)
	ROStructType     string //RO结构体名称
	ROPkgType        string
	//StructType string // 自定义结构类型
	//ModuleName   string // 功能模块名
	CfgDefine string
}
