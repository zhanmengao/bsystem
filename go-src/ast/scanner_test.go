package ast

import "go/token"

import (
	"fmt"
	"go/scanner"
	"testing"
)

func TestScanner(t *testing.T) {
	src := []byte(`package main
import "fmt"
//comment
func main() {
  fmt.Println("Hello, world!")
}
`)
	var s scanner.Scanner
	fSet := token.NewFileSet()
	file := fSet.AddFile("", fSet.Base(), len(src))
	//注意没有扫描出注释，需要的话要将s.Init的最后一个参数改为scanner.ScanComments
	s.Init(file, src, nil, 0)

	for {
		pos, tok, lit := s.Scan()				//token就是一堆定义好的枚举类型，对于每种类型的字面值都有对应的token。
		fmt.Printf("%-6s%-8s%q\n", fSet.Position(pos), tok, lit)

		if tok == token.EOF {
			break
		}
	}
}
