package ast

import (
	"go/ast"
	"go/parser"
	"go/token"
	"testing"
)
/*
   当源码被扫描成token之后，结果就被传递给了Parser
   将token转换为抽象语法树（AST）
   编译时的错误也是在这个时候报告的
 */
func TestParserAST(t *testing.T) {
	src := []byte(`/*comment0*/
package main
import "fmt"
//comment1
/*comment2*/
func main() {
  fmt.Println("Hello, world!")
}
`)

	// Create the AST by parsing src.
	fset := token.NewFileSet() // positions are relative to fset
	//同样注意没有扫描出注释，需要的话要将parser.ParseFile的最后一个参数改为parser.ParseComments
	f, err := parser.ParseFile(fset, "", src, 0)
	if err != nil {
		panic(err)
	}

	// Print the AST.
	ast.Print(fset, f)
}

//type File struct {
//    Doc        *CommentGroup   // associated documentation; or nil
//    Package    token.Pos       // position of "package" keyword
//    Name       *Ident          // package name
//    Decls      []Decl          // top-level declarations; or nil
//    Scope      *Scope          // package scope (this file only)
//    Imports    []*ImportSpec   // imports in this file
//    Unresolved []*Ident        // unresolved identifiers in this file
//    Comments   []*CommentGroup // list of all comments in the source file
//}
