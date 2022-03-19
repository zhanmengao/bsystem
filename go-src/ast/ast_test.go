package ast

import (
	"go/ast"
	"go/parser"
	"go/token"
	"path/filepath"
	"testing"
)

func TestAst(t *testing.T) {
	src := `
package main
func main(){
    println("hello world")
}
    `
	//创建用于解析源文件的对象
	fileset := token.NewFileSet()
	//解析源文件，返回ast.File原始文档类型的结构体。
	astFile, err := parser.ParseFile(fileset, "", src, 0)
	if err != nil {
		panic(err)
	}
	//查看日志打印
	ast.Print(fileset, astFile)
}

func TestAst2(t *testing.T) {
	absPath, err := filepath.Abs("./demo/demo.go")
	if err != nil {
		panic(absPath)
	}
	//创建用于解析源文件的对象
	fSet := token.NewFileSet()
	//解析源文件，返回ast.File原始文档类型的结构体。
	f, err := parser.ParseFile(fSet, absPath, nil, parser.AllErrors)
	if err != nil {
		panic(err)
	}
	//递归调用逐一打印节点
	ast.Inspect(f, func(n ast.Node) bool {
		ast.Print(fSet, n)
		return true
	})
}
