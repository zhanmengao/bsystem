package generator

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"io/ioutil"
	"os"
	"path"
)

func Run(pbGoDir, pbPackage, pkgName, outputDir string) {

	gen, err := NewGenerator()
	if err != nil {
		panic(err)
	}

	tfs := token.NewFileSet()
	pkg, err := parser.ParseDir(tfs, pbGoDir, func(info os.FileInfo) bool {
		return true
	}, parser.ParseComments)
	if err != nil {
		panic(err)
	}
	ps := NewParser()

	for _, v := range pkg {
		ast.Walk(ps, v)
	}
	_ = os.RemoveAll(outputDir)
	if err = os.MkdirAll(outputDir, os.ModePerm); err != nil {
		panic(err)
	}

	ps.PbPackage = pbPackage
	//拷贝manager文件
	managerList, err := loadManager()
	if err != nil {
		panic(err)
	}
	for name, body := range managerList {
		if err = ioutil.WriteFile(path.Join(outputDir, name), body, 0644); err != nil {
			return
		}
	}

	err = ps.BuildLoader(gen, ps.PbPackage,pkgName, outputDir, ps.comments, ps.typeInfo)
	if err != nil {
		panic(err.Error())
	}

	fmt.Println("generate success!")
}
