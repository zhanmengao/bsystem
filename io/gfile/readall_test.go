package gio

import (
	"fmt"
	"io/ioutil"
	"os"
	"testing"
)
var(
	path string
)
func init(){
	path,_ = os.Getwd()
	path+="/readall_test.go"
}
func TestReadAll(t *testing.T) {
	file, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer file.Close()
	content, err := ioutil.ReadAll(file)
	fmt.Println(string(content))
}

func TestReadAll2(t *testing.T) {
	content ,err :=ioutil.ReadFile(path)
	if err !=nil {
		panic(err)
	}
	fmt.Println(string(content))
}
