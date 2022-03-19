package main

import "go/ast"

type scan struct {
	PkgName     string
	ImportProto string
	CfgList     []*ast.TypeSpec
	TypeMap     map[string]*ast.TypeSpec
	MetaList    []*roMeta
	AlreadyGen  map[string]struct{}
}

type roMeta struct {
	ProtoType string
	ROType    string
	//以下key均为成员名称
	NormalList map[string]*filterMeta //内嵌普通成员
	StructList map[string]*roMeta     //内嵌结构体的情况，递归处理

	NormalSliceList map[string]*filterMeta //内嵌普通成员数组
	StructSliceList map[string]*roMeta     //内嵌结构体数组
	NormalMapList   map[string]*mapMeta    //内嵌普通成员map
	StructMapList   map[string]*mapROMate  //内嵌结构体成员map

	SelfList      []string          //内嵌自己的结构体成员
	SelfSliceList []string          //内嵌自己的数组成员
	SelfMapList   map[string]string //内嵌自己的map成员
	FilterList    []string          //成员列表
}

func newROMeta(ProtoType string) *roMeta {
	meta := &roMeta{
		NormalList:      make(map[string]*filterMeta, 0),
		StructList:      make(map[string]*roMeta, 0),
		NormalSliceList: make(map[string]*filterMeta, 0),
		StructSliceList: make(map[string]*roMeta, 0),
		NormalMapList:   make(map[string]*mapMeta, 0),
		StructMapList:   make(map[string]*mapROMate, 0),
		SelfList:        make([]string, 0, 0),
		SelfSliceList:   make([]string, 0, 0),
		SelfMapList:     make(map[string]string, 0),
		FilterList:      make([]string, 0),
		ProtoType:       ProtoType,
	}
	meta.ROType = meta.ProtoType + "RO"
	return meta
}

type filterMeta struct {
	FilterType string
}

func newFilterMeta(FilterType string) *filterMeta {
	return &filterMeta{
		FilterType: FilterType,
	}
}

type mapMeta struct {
	KeyType   string
	ValueType string
}

func newMapMeta(KeyType, ValueType string) *mapMeta {
	return &mapMeta{
		ValueType: ValueType,
		KeyType:   KeyType,
	}
}

type mapROMate struct {
	KeyType string
	*roMeta
}

func newROMapMeta(KeyType, ValueType string) *mapROMate {
	meta := &mapROMate{
		KeyType: KeyType,
		roMeta:  newROMeta(ValueType),
	}
	return meta
}

type genInfo struct {
	FilterName string
	KeyType    string
	FilterType string
	ROType     string
}
