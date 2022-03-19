package ast

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"strings"
	"testing"
)

type Visitor int

func (v Visitor) Visit(n ast.Node) ast.Visitor {
	if n == nil {
		return nil
	}
	//递归地打印出所有的token节点
	fmt.Printf("%s%T\n", strings.Repeat("\t", int(v)), n)
	return v + 1
}

func TestASTWalk(t *testing.T) {
	// Create the AST by parsing src.
	fset := token.NewFileSet() // positions are relative to fset
	f, err := parser.ParseFile(fset, "", "package main; var a = 3", parser.ParseComments)
	if err != nil {
		panic(err)
	}
	var v Visitor
	ast.Walk(v, f)
}
