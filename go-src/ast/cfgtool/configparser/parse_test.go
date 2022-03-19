package main

import (
	"fmt"
	"testing"
)

func TestParse(t *testing.T) {
	def, cfg, err := parseComment("DemoAb", "// @cfg:DemoAbConfig|struct:DemoAbConfig #struct")
	if err != nil {
		panic(err)
	}
	fmt.Printf("def = %v \n", def)
	fmt.Printf("cfg = %v \n", cfg)
}
