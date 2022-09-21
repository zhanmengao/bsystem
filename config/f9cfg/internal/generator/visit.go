package generator

import (
	"go/ast"
	"go/token"
	"strings"
)

func (p *CustomParser) Visit(n ast.Node) ast.Visitor {
	switch n := n.(type) {
	case *ast.Package:
		return p
	case *ast.File:
		return p
	case *ast.GenDecl:
		if n.Tok == token.TYPE {
			p.msgStart = true
			if n.Doc != nil {
				for _, comment := range n.Doc.List {
					if strings.Contains(comment.Text, "@cfg:") {
						p.msgComments = comment.Text
					}
				}
			}
			return p
		}
	case *ast.TypeSpec:
		if strings.Contains(n.Name.Name, "ConfigAry") && !strings.Contains(n.Name.Name, "ConfigAryRO") {
			if p.msgStart {
				if p.msgComments != "" {
					p.comments[n.Name.Name] = p.msgComments
				}
				p.msgStart = false
				p.msgComments = ""
			}
		}
		if _, ok := n.Type.(*ast.StructType); ok {
			p.typeInfo[n.Name.Name] = n
		}
	}

	return p
}
