package main

import (
	"go/ast"
	"strings"
)

func (s *scan) Visit(node ast.Node) ast.Visitor {
	switch nn := node.(type) {
	case *ast.GenDecl:
		spec := nn.Specs
		if len(spec) == 1 {
			ss := spec[0]
			switch tn := ss.(type) {
			case *ast.TypeSpec:
				name := tn.Name.Name
				if _, ok := tn.Type.(*ast.StructType); ok {
					if strings.HasSuffix(name, "ConfigAry") && name != "EnumConfigAry" {
						s.CfgList = append(s.CfgList, tn)
					}
					if s.TypeMap == nil {
						s.TypeMap = make(map[string]*ast.TypeSpec)
					}
					s.TypeMap[name] = tn
				}
			}
		}
	}
	return s
}
