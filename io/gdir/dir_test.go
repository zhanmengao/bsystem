package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"
	"syscall"
	"testing"
)

func TestDir(t *testing.T) {
	home, found := syscall.Getenv("GOPATH")
	if !found {
		fmt.Printf("HOME not found \n")
		t.FailNow()
	}
	paths := strings.SplitN(home, ";", 2)
	dirs, err := ioutil.ReadDir(paths[0])
	if err != nil {
		log.Fatalln(err)
	}
	for _, file := range dirs {
		fmt.Printf("info = %v \n", file)
	}
}

func TestFilePath(t *testing.T) {
	home, err := os.Getwd()
	if err != nil {
		t.Fatal(err)
	}
	str := filepath.Join(home, "*.go")
	fmt.Printf("join str = %s \n", str)
	filepathNames, err := filepath.Glob(str) //获取所有table开头的文件
	if err != nil {
		t.Fatal(err)
	}
	for _, v := range filepathNames {
		fmt.Println(v)
	}
}
