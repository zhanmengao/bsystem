package meta

import (
	"fmt"
	"go/ast"
	"strings"
)

func GetFieldName(field *ast.Field, pbPkg string) (tpName string, fieldName string) {
	fieldName = field.Names[0].Name
	switch vv := field.Type.(type) {
	case *ast.ArrayType:
		switch elt := vv.Elt.(type) {
		case *ast.StarExpr:
			ident, ok := elt.X.(*ast.Ident)
			if !ok {
				ident = elt.X.(*ast.SelectorExpr).X.(*ast.Ident)
			}
			tpName = ident.Name
			return
		case *ast.Ident:
			tpName = elt.Name
			return
		}
	case *ast.MapType:
		switch value := vv.Value.(type) {
		case *ast.Ident:
			fieldName = value.Name
			return
		case *ast.StarExpr:
			//小心内嵌自己的情况
			ident := value.X.(*ast.Ident)
			tpName = ident.Name
			return
		}
	case *ast.Ident:
		tpName = vv.Name
		switch vv.Name {
		case "string", "int32", "int64", "uint32", "uint64", "float32", "float64", "float":
		default:
			tpName = fmt.Sprintf("%s.%s", pbPkg, tpName)
		}
		return
	case *ast.StarExpr:
		ident := vv.X.(*ast.Ident)
		//小心内嵌自己的情况
		tpName = ident.Name
	case *ast.StructType:
	default:
		fmt.Println("default : ", vv)
	}
	return
}

func genMapMeta(typeMap map[string]*ast.TypeSpec, fl *ast.FieldList, root *MapROMate) {
	GenMeta(typeMap, fl, root.RoMeta)
}

func GenMeta(typeMap map[string]*ast.TypeSpec, fl *ast.FieldList, root *RoMeta) {
	for _, ff := range fl.List {
		name := ff.Names[0].Name
		if strings.HasPrefix(name, "XXX_") {
			continue
		}
		root.FilterList = append(root.FilterList, name)
		switch vv := ff.Type.(type) {
		case *ast.ArrayType:
			switch elt := vv.Elt.(type) {
			case *ast.StarExpr:
				ident, ok := elt.X.(*ast.Ident)
				if !ok {
					ident = elt.X.(*ast.SelectorExpr).X.(*ast.Ident)
				}
				filterType := ident.Name
				//小心内嵌自己的情况
				if root.ProtoType == filterType {
					root.SelfSliceList = append(root.SelfSliceList, name)
				} else {
					ts := typeMap[filterType]
					filter := NewROMeta(filterType)
					GenMeta(typeMap, ts.Type.(*ast.StructType).Fields, filter)
					root.StructSliceList[name] = filter
				}
			case *ast.Ident:
				filter := NewFilterMeta(elt.Name)
				root.NormalSliceList[name] = filter
			}
		case *ast.MapType:
			//key只能是基础数据类型
			key := vv.Key.(*ast.Ident)
			switch value := vv.Value.(type) {
			case *ast.Ident:
				root.NormalMapList[name] = NewMapMeta(key.Name, value.Name)
			case *ast.StarExpr:
				//小心内嵌自己的情况
				ident := value.X.(*ast.Ident)
				filterName := ident.Name
				//小心内嵌自己的情况
				if root.ProtoType == filterName {
					root.SelfMapList[name] = key.Name
				} else {
					ts := typeMap[filterName]
					filter := NewROMapMeta(key.Name, filterName)
					genMapMeta(typeMap, ts.Type.(*ast.StructType).Fields, filter)
					root.StructMapList[name] = filter
				}
			}
		case *ast.Ident:
			filter := NewFilterMeta(vv.Name)
			root.NormalList[name] = filter
		case *ast.StarExpr:
			ident := vv.X.(*ast.Ident)
			//小心内嵌自己的情况
			if ident.Name != root.ProtoType {
				ts := typeMap[ident.Name]
				filter := NewROMeta(ident.Name)
				GenMeta(typeMap, ts.Type.(*ast.StructType).Fields, filter)
				root.StructList[name] = filter
			} else {
				root.SelfList = append(root.SelfList, name)
			}
		default:
			fmt.Println(vv)
		}
	}
}
