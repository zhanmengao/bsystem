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
	msgStart    bool
	msgComments []string

	comments map[string][]string
}

func NewParser(fp string) *CustomParser {
	return &CustomParser{filepath: fp, comments: map[string][]string{}}
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

func (p *CustomParser) Export() map[string][]string {
	return p.comments
}

func (p *CustomParser) Visit(n ast.Node) ast.Visitor {
	switch n := n.(type) {
	case *ast.Package:
		return p
	case *ast.File:
		return p
	case *ast.GenDecl:
		if n.Tok == token.TYPE {
			defer func() {
				p.msgStart = true

				if n.Doc != nil {
					for _, comment := range n.Doc.List {
						if strings.Contains(comment.Text, "@cfg:") {
							if p.msgComments == nil {
								p.msgComments = make([]string, 0, 1)
							}
							p.msgComments = append(p.msgComments, comment.Text)
						}
					}
				}
			}()
			return p
		}
	case *ast.TypeSpec:
		if strings.Contains(n.Name.Name, "ConfigAry") {
			//We have hit the inputs struct
			//fmt.Println("comment doc", n.Comment)
			key := strings.Replace(n.Name.Name, "ConfigAry", "", 1)
			if p.msgStart {
				p.comments[key] = p.msgComments[:]
				p.msgStart = false
				p.msgComments = make([]string, 0)
			}
		}
	}

	return nil
}
