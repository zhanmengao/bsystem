package main

import (
	"fmt"
	"os"
	"path/filepath"
	"runtime"
)

func main() {
	fmt.Println(runtime.Caller(0))
	var coreRoot string
	if _, file, _, ok := runtime.Caller(0); ok {
		coreRoot, _ = filepath.Split(file)
	}

	// 参数解析
	pbPath := os.Args[1]
	outputDir := os.Args[2]
	pbPackage := os.Args[3]

	filepathNames, err := filepath.Glob(filepath.Join(pbPath, "*.pb.go")) //获取所有proto文件
	if err != nil {
		panic(err)
	}

	for i, filePath := range filepathNames {
		fmt.Println("parse", filepathNames[i]) //打印path

		// 解析注释获取table结构声明
		ps := NewParser(filePath)
		err = ps.ParseComments()
		if err != nil {
			panic(err.Error())
		}
		comments := ps.Export()
		if len(comments) == 0 {
			continue
		}
		Build(comments, filepath.Join(coreRoot, "template"), outputDir, pbPackage)
	}
}
