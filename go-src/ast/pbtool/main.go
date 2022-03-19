package main

import (
	"bytes"
	"html/template"
	"io/ioutil"
	"os"
	"path/filepath"
)

type errorParam struct {
	PackageName string
}

func main() {
	root := os.Args[1]
	tmp, err := template.ParseFiles(filepath.Join(root, "build_error.tpl"))
	if err != nil {
		panic(err)
	}
	filePath := filepath.Join(root, "build_error.go")
	param := &errorParam{
		PackageName: os.Args[2],
	}
	buf := bytes.NewBufferString("")
	if err = tmp.Execute(buf, param); err != nil {
		panic(err)
	}
	if err = ioutil.WriteFile(filePath, buf.Bytes(), 0644); err != nil {
		return
	}
}
