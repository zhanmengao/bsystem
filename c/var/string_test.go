package main

import (
	"fmt"
	"testing"
)

func GetConstName()string{
	return "lan"
}


func GetName()string{
	name := "lan"
	fmt.Printf("GetName addr %p \n",&name)
	return name
}
func GetPName()*string{
	name := "lan"
	fmt.Printf("GetPName addr %p \n",&name)
	return &name
}
func TestString(t *testing.T) {
	cst := GetConstName()
	name := GetName()
	pname := GetPName()
	fmt.Printf("cst addr %p  \t\t name addr %p  pname addr %p = %s \n", &cst, &name,pname,*pname)
}
