package ast

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"io/ioutil"
	"log"
	"strings"
	"testing"
)

func TestNode(t *testing.T){
	cp := &FindContext{}
	buf, err := ioutil.ReadFile("./demo/test2.go")
	if err != nil {
		log.Fatalln(err)
		return
	}
	fSet := token.NewFileSet()
	f, err := parser.ParseFile(fSet, "", string(buf), parser.ParseComments)
	if err != nil {
		log.Fatalln(err)
		return
	}
	ast.Walk(cp, f)
	fmt.Printf("%v \n", cp)
}


type CustomParser struct {
	msgStart    bool
	msgComments []string

	comments map[string][]string
}

func TestWalk(t *testing.T) {
	cp := &CustomParser{
		comments: make(map[string][]string),
	}
	buf, err := ioutil.ReadFile("./demo/table_server.pb.go")
	if err != nil {
		log.Fatalln(err)
		return
	}
	fSet := token.NewFileSet()
	f, err := parser.ParseFile(fSet, "", string(buf), parser.ParseComments)
	if err != nil {
		log.Fatalln(err)
		return
	}
	ast.Walk(cp, f)
	fmt.Printf("%v \n", cp)
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
				p.comments[key] = p.msgComments
				p.msgStart = false
				p.msgComments = make([]string, 0)
			}
		}
	}

	return nil
}