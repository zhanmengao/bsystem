package main

import (
	"flag"
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"log"
	"os"
	"path"
)

var (
	dir       string
	s         *scan
)

func main() {
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	flag.StringVar(&dir, "d", "", "proto path to parse")
	flag.Parse()
	st, err := os.Stat(dir)
	if err != nil {
		panic(err)
	}
	if !st.IsDir() {
		panic(dir + " not a dir")
	}
	if err = os.Remove(path.Join(dir, "autogen_readonly.pb.go")); err != nil {
		log.Println(err)
	}
	tfs := token.NewFileSet()
	pkg, err := parser.ParseDir(tfs, dir, func(info os.FileInfo) bool {
		log.Printf("parsing file:%s\n", info.Name())
		return true
	}, parser.AllErrors)
	if err != nil {
		panic(err)
	}
	log.Printf("total size :%d\n", len(pkg))
	s = &scan{
		MetaList:   make([]*roMeta, 0),
		AlreadyGen: make(map[string]struct{}),
	}
	if len(pkg) != 1 {
		panic("package illegal")
	}
	for k, v := range pkg {
		s.PkgName = k
		ast.Walk(s, v)
	}

	log.Printf("package name is:%s\n", s.PkgName)

	for _, v := range s.CfgList {
		log.Printf("found config type :%s\n", v.Name.Name)
		meta := newROMeta(v.Name.Name)
		genMeta(v.Type.(*ast.StructType).Fields, meta)
		s.MetaList = append(s.MetaList, meta)
	}

	//开始生成模板
	if err = genTemplateList(path.Join(dir, "autogen_readonly.pb.go")); err != nil {
		log.Fatal(err)
	}
	fmt.Println(s.MetaList)
}
