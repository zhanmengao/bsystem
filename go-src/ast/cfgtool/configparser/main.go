package main

import (
	"fmt"
	"go/format"
	"go/parser"
	"go/token"
	"io/ioutil"
	"log"
	"os"
	"path/filepath"
	"strings"
)

func main() {
	fmt.Println(os.Getwd())
	fmt.Println(os.Args)
	var err error

	// 参数解析
	dirPath := os.Args[1]
	outputPath := os.Args[2]
	tplPath := os.Args[3]

	var pbPackage string

	tfs := token.NewFileSet()
	pkg, err := parser.ParseDir(tfs, dirPath, func(info os.FileInfo) bool {
		log.Printf("parsing file:%s\n", info.Name())
		return true
	}, parser.AllErrors)
	if err != nil {
		panic(err)
	}
	log.Printf("total size :%d\n", len(pkg))
	if len(pkg) != 1 {
		panic("package illegal")
	}
	for k, _ := range pkg {
		pbPackage = k
	}

	filepathNames, err := filepath.Glob(filepath.Join(dirPath, "*table*")) //获取所有table开头的文件
	if err != nil {
		panic(err)
	}

	gen := NewGenerator(tplPath)

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
		err = BuildLoader(gen, pbPackage, outputPath, comments)
		if err != nil {
			panic(err.Error())
		}
	}

	fmt.Println("generate success!")
}

func BuildLoader(fg *FileGenerator, ProtoPackage string, outputPath string, comments map[string][]string) (err error) {
	if err = os.MkdirAll(outputPath, os.ModePerm); err != nil {
		return err
	}

	sheetConfigs := make(map[string]*SheetConfig)
	for sheet, msgComments := range comments {
		for _, comment := range msgComments {
			// 解析注释
			var def *ConfigDefine
			var sheetConfig *SheetConfig
			def, sheetConfig, err = parseComment(sheet, comment)
			if err != nil {
				return
			}

			sheetConfigs[def.ObjName] = sheetConfig
		}
	}

	for buildClassName, configs := range sheetConfigs {
		var buf []byte
		buf, err = fg.Build(buildClassName, ProtoPackage, configs)
		if err != nil {
			return
		}
		cpy := make([]byte, len(buf))
		copy(cpy, buf)

		//fmt.Println(string(buf))
		//os.Exit(2)

		buf, err = format.Source(buf)
		if err != nil {
			fmt.Printf("code : %s \n", cpy)
			return
		}

		fileName := ToUnderline(buildClassName)

		filePath := filepath.Join(outputPath, "gen_"+fileName+".go")

		fmt.Println("gen", buildClassName, filePath)
		if err = ioutil.WriteFile(filePath, buf, 0644); err != nil {
			return
		}
	}
	return
}

func parseComment(sheet, comment string) (def *ConfigDefine, sheetConfig *SheetConfig, err error) {
	def = &ConfigDefine{
		Sheet: sheet,
		//T: defType,
		//CfgStruct: customCfgName,
		//Desc: desc,
		//
		//ObjName: buildClassName,
		//
		//Params: params,
	}

	params := strings.Split(comment, Separator)
	params[0] = strings.Trim(params[0], "// ")
	tmpArr := strings.Split(params[0], ":")

	def.ObjName = tmpArr[1]
	if len(tmpArr) == 3 {
		def.FieldName = tmpArr[2]
	}

	if len(params) < 2 {
		err = fmt.Errorf("comment error Separator len")
		return
	}

	//说明
	last := params[len(params)-1]
	if strings.Contains(last, "#") {
		tmpArr := strings.Split(last, "#")
		params[len(params)-1] = tmpArr[0]
		def.Desc = tmpArr[1]

	}
	for i, p := range params {
		params[i] = strings.TrimSpace(p)
	}

	declares := strings.Split(params[1], ":")
	// 定义类型
	def.T = declares[0]
	// 自定义所用的pb cfg name
	if len(declares) > 1 {
		def.CfgStruct = declares[1]
	}

	def.Params = params[2:]

	// 生成不同类型的配置
	if builder, ok := ConfigBuilders[def.T]; ok {
		sheetConfig, err = builder.Build(def)
		if err != nil {
			return
		}
	}

	return
}
