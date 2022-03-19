package main

import (
	"bytes"
	"fmt"
	"log"
	"os"
	"path"
	"path/filepath"
	"strings"
	"text/template"
)

type buildInfo struct {
	Name         string
	ProtoPackage string
	PbType       string
	Protocol     string
}

func Build(meta map[string]string, templateDir string, outputDir string, pbPackage string) {
	for k, v := range meta {
		info := &buildInfo{
			ProtoPackage: pbPackage,
			PbType:       k,
		}
		v = strings.ReplaceAll(v, "//@fortest:", "")
		ss := strings.Split(v, "|")
		for _, s := range ss {
			sp := strings.Split(s, "-")
			if len(sp) < 2 {
				continue
			}
			switch sp[0] {
			case "Name":
				info.Name = sp[1]
			case "Protocol":
				info.Protocol = sp[1]
			}
		}
		log.Printf("build ftest %v \n", info)
		build(outputDir, templateDir, info)
	}
}

func build(outputDir string, dir string, info *buildInfo) {
	tmp, err := template.ParseFiles(path.Join(dir, fmt.Sprintf("ftest_%s.tpl", info.Protocol)))
	if err != nil {
		panic(err)
	}
	buf := bytes.NewBufferString("")
	if err = tmp.Execute(buf, info); err != nil {
		panic(err)
	}

	filePath := filepath.Join(outputDir, "autogen_"+info.Name+"_fortest.go")
	pf, err := os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		panic(err)
	}
	defer pf.Close()
	if _, err = pf.Write(buf.Bytes()); err != nil {
		panic(err)
	}
}
