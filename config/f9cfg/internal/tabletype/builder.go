package tabletype

const (
	Mark        = "@"
	PackagePath = "pb."
)

const (
	TypeStruct       = "Struct"
	TypeMap          = "Map"
	TypeList         = "List"
	TypeGroup        = "Group"
	TypeGroupMap     = "GroupMap"
	TypeGroupMapList = "GroupMapList"
	Separator        = "|"
)

var ConfigBuilders map[string]IConfigBuilder

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
