package main

import (
	"go/ast"
	"go/parser"
	"go/token"
	"io/ioutil"
	"strings"
)

// 解析文件所有路径

type CustomParser struct {
	filepath    string
	msgComments string
	packageName string

	comments map[string]string
}

func NewParser(fp string) *CustomParser {
	return &CustomParser{
		filepath: fp,
		comments: map[string]string{},
	}
}

// ParseComments 解析注释获取table结构声明
func (p *CustomParser) ParseComments() (err error) {
	buf, err := ioutil.ReadFile(p.filepath)
	if err != nil {
		return
	}
	fSet := token.NewFileSet()
	f, err := parser.ParseFile(fSet, "", string(buf), parser.ParseComments)
	if err != nil {
		return
	}
	ast.Walk(p, f)
	return
}

func (p *CustomParser) Export() map[string]string {
	return p.comments
}

func (p *CustomParser) Visit(n ast.Node) ast.Visitor {
	switch n := n.(type) {
	case *ast.Package:
		return p
	case *ast.File:
		p.packageName = n.Name.Name
		p.packageName = strings.ReplaceAll(p.packageName, "_", ".")
		return p
	case *ast.GenDecl:
		if n.Tok == token.TYPE {
			if n.Doc != nil {
				for _, comment := range n.Doc.List {
					if strings.Contains(comment.Text, "@fortest:") {
						p.msgComments = comment.Text
					}
				}
			}
			return p
		}
	case *ast.TypeSpec:
		if len(p.msgComments) > 0 {
			p.comments[n.Name.Name] = p.msgComments
			p.msgComments = ""
		}
	}

	return nil
}
