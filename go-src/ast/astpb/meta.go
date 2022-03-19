package main

import (
	"fmt"
	"go/ast"
	"strings"
)

func genMapMeta(fl *ast.FieldList, root *mapROMate) {
	genMeta(fl, root.roMeta)
}

func genMeta(fl *ast.FieldList, root *roMeta) {
	for _, ff := range fl.List {
		name := ff.Names[0].Name
		if strings.HasPrefix(name, "XXX_") {
			continue
		}
		root.FilterList = append(root.FilterList,name)
		switch vv := ff.Type.(type) {
		case *ast.ArrayType:
			switch elt := vv.Elt.(type) {
			case *ast.StarExpr:
				ident := elt.X.(*ast.Ident)
				filterName := ident.Name
				//小心内嵌自己的情况
				if root.ProtoType == filterName {
					root.SelfSliceList = append(root.SelfSliceList, name)
				} else {
					ts := s.TypeMap[filterName]
					filter := newROMeta(filterName)
					genMeta(ts.Type.(*ast.StructType).Fields, filter)
					root.StructSliceList[name] = filter
				}
			case *ast.Ident:
				filter := newFilterMeta(elt.Name)
				root.NormalSliceList[name] = filter
			}
		case *ast.MapType:
			//key只能是基础数据类型
			key := vv.Key.(*ast.Ident)
			switch value := vv.Value.(type) {
			case *ast.Ident:
				root.NormalMapList[name] = newMapMeta(key.Name, value.Name)
			case *ast.StarExpr:
				//小心内嵌自己的情况
				ident := value.X.(*ast.Ident)
				filterName := ident.Name
				//小心内嵌自己的情况
				if root.ProtoType == filterName {
					root.SelfMapList[name] = key.Name
				} else {
					ts := s.TypeMap[filterName]
					filter := newROMapMeta(key.Name, filterName)
					genMapMeta(ts.Type.(*ast.StructType).Fields, filter)
					root.StructMapList[name] = filter
				}
			}
		case *ast.Ident:
			filter := newFilterMeta(vv.Name)
			root.NormalList[name] = filter
		case *ast.StarExpr:
			ident := vv.X.(*ast.Ident)
			//小心内嵌自己的情况
			if ident.Name != root.ProtoType {
				ts := s.TypeMap[ident.Name]
				filter := newROMeta(ident.Name)
				genMeta(ts.Type.(*ast.StructType).Fields, filter)
				root.StructList[name] = filter
			} else {
				root.SelfList = append(root.SelfList, name)
			}
		default:
			fmt.Println(vv)
		}
	}
}
