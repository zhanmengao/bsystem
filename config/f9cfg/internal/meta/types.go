package meta

type FilterMeta struct {
	FilterType string
}

func NewFilterMeta(FilterType string) *FilterMeta {
	return &FilterMeta{
		FilterType: FilterType,
	}
}

type MapMeta struct {
	KeyType   string
	ValueType string
}

func NewMapMeta(KeyType, ValueType string) *MapMeta {
	return &MapMeta{
		ValueType: ValueType,
		KeyType:   KeyType,
	}
}

type MapROMate struct {
	KeyType string
	*RoMeta
}

func NewROMapMeta(KeyType, ValueType string) *MapROMate {
	meta := &MapROMate{
		KeyType: KeyType,
		RoMeta:  NewROMeta(ValueType),
	}
	return meta
}

type RoMeta struct {
	ProtoType string
	ROType    string
	//以下key均为成员名称
	NormalList map[string]*FilterMeta //内嵌普通成员
	StructList map[string]*RoMeta     //内嵌结构体的情况，递归处理

	NormalSliceList map[string]*FilterMeta //内嵌普通成员数组
	StructSliceList map[string]*RoMeta     //内嵌结构体数组
	NormalMapList   map[string]*MapMeta    //内嵌普通成员map
	StructMapList   map[string]*MapROMate  //内嵌结构体成员map

	SelfList      []string          //内嵌自己的结构体成员
	SelfSliceList []string          //内嵌自己的数组成员
	SelfMapList   map[string]string //内嵌自己的map成员
	FilterList    []string          //成员列表
}

func NewROMeta(ProtoType string) *RoMeta {
	meta := &RoMeta{
		NormalList:      make(map[string]*FilterMeta, 0),
		StructList:      make(map[string]*RoMeta, 0),
		NormalSliceList: make(map[string]*FilterMeta, 0),
		StructSliceList: make(map[string]*RoMeta, 0),
		NormalMapList:   make(map[string]*MapMeta, 0),
		StructMapList:   make(map[string]*MapROMate, 0),
		SelfList:        make([]string, 0, 0),
		SelfSliceList:   make([]string, 0, 0),
		SelfMapList:     make(map[string]string, 0),
		FilterList:      make([]string, 0),
		ProtoType:       ProtoType,
	}
	meta.ROType = meta.ProtoType + "RO"
	return meta
}

func (m *RoMeta) GetParamType(filterName string) string {
	if data, exist := m.NormalList[filterName]; exist {
		return data.FilterType
	}
	if data, exist := m.StructList[filterName]; exist {
		return data.ProtoType
	}
	if data, exist := m.NormalSliceList[filterName]; exist {
		return "[]" + data.FilterType
	}
	if data, exist := m.StructSliceList[filterName]; exist {
		return "[]*" + data.ProtoType
	}
	if data, exist := m.NormalMapList[filterName]; exist {
		return data.ValueType
	}
	if data, exist := m.StructMapList[filterName]; exist {
		return data.ProtoType
	}
	for _, v := range m.SelfList {
		if v == filterName {
			return m.ProtoType
		}
	}
	for _, v := range m.SelfSliceList {
		if v == filterName {
			return "[]*" + m.ProtoType
		}
	}
	for k, _ := range m.SelfMapList {
		if k == filterName {
			return m.ProtoType
		}
	}
	return ""
}
