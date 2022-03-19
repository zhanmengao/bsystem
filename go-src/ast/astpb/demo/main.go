package main

import (
	"bytes"
	"flag"
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"log"
	"os"
	"strings"
	"time"
)

var (
	dir string
)

type scan struct {
	pkgName string
	cfgList []*ast.TypeSpec
	typeMap map[string]*ast.TypeSpec
}

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
						s.cfgList = append(s.cfgList, tn)
					}
					if s.typeMap == nil {
						s.typeMap = make(map[string]*ast.TypeSpec)
					}
					s.typeMap[name] = tn
				}
			}
		}
	}
	return s
}

func main() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	flag.StringVar(&dir, "d", "", "default path to parse")
	flag.Parse()
	st, err := os.Stat(dir)
	if err != nil {
		panic(err)
	}
	if !st.IsDir() {
		panic(dir + " not a dir")
	}
	tfs := token.NewFileSet()
	start := time.Now()
	pkg, err := parser.ParseDir(tfs, dir, func(info os.FileInfo) bool {
		log.Printf("parsing file:%s\n", info.Name())
		return true
	}, parser.AllErrors)
	if err != nil {
		panic(err)
	}
	log.Printf("total size :%d\n", len(pkg))
	//for k, v := range pkg {
	//	//log.Printf("package name is:%s\n", k)
	//	//for fileName, fAst := range v.Files {
	//	//	if strings.Index(fileName, "table.pb.go") != -1 {
	//	//		ast.Walk(func() {}, fAst)
	//	//	}
	//	//}
	//
	//}
	s := &scan{}
	if len(pkg) != 1 {
		panic("package illegal")
	}
	for k, v := range pkg {
		s.pkgName = k
		ast.Walk(s, v)
	}
	diff := time.Now().Sub(start)
	log.Printf("total cost time:%s\n", diff.String())

	log.Printf("package name is:%s\n", s.pkgName)
	buffer := &bytes.Buffer{}
	for _, v := range s.cfgList {
		//ast.Print(tfs, v)
		log.Printf("found config type :%s\n", v.Name.Name)
		fmt.Fprintf(buffer, "Fields:\n")
		for idx, ff := range v.Type.(*ast.StructType).Fields.List {
			name := ff.Names[0].Name
			if strings.HasPrefix(name, "XXX_") {
				continue
			}
			fmt.Fprintf(buffer, "idx:%d, name:%s, type:%s\n",
				idx, name, ff.Type.(*ast.ArrayType).Elt.(*ast.StarExpr).X.(*ast.Ident).Name)
		}
		log.Println(string(buffer.Bytes()))
		buffer.Reset()
	}

	for k, v := range s.typeMap {
		log.Printf("type:%s, structName :%s\n", k, v.Name.Name)
	}

	err = ast.Print(tfs, s.typeMap["AppEvent"])
	if err != nil {
		panic(err)
	}
}
