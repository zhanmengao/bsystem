package main

import (
	"fmt"
	"go/format"
	"io/ioutil"
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
	deepCopy := len(os.Args) > 4 && os.Args[4] == "deepCopy"
	abTest := len(os.Args) > 5 && os.Args[5] == "abTest"

	filepathNames, err := filepath.Glob(filepath.Join(dirPath, "table*"))				//获取所有table开头的文件
	if err != nil {
		panic(err)
	}

	gen := tool.NewGenerator(tplPath)

	for i, filePath := range filepathNames {
		fmt.Println("parse", filepathNames[i]) //打印path

		// 解析注释获取table结构声明
		ps := tool.NewParser(filePath)
		err = ps.ParseComments()
		if err != nil {
			panic(err.Error())
		}
		comments := ps.Export()
		if len(comments) == 0 {
			panic("have no commets")
		}
		err = BuildLoader(gen, outputPath, comments, deepCopy, abTest)
		if err != nil {
			panic(err.Error())
		}
	}

	fmt.Println("generate success!")
}

func BuildLoader(gen *tool.FileGenerator, outputPath string, comments map[string][]string, deepCopy, abTest bool) (err error) {
	if err := os.MkdirAll(outputPath, os.ModePerm); err != nil {
		return err
	}

	ab := "false"
	if abTest {
		ab = "true"
	}
	sheetConfigs := make(map[string][]*tool.SheetConfig)
	for sheet, msgComments := range comments {
		for _, comment := range msgComments {
			// 解析注释
			var def *tool.ConfigDefine
			var sheetConfig *tool.SheetConfig
			def, sheetConfig, err = parseComment(sheet, comment)
			if err != nil {
				return
			}

			sheetConfig.DeepCopy = deepCopy
			sheetConfig.AbTest = ab
			sheetConfigs[def.ObjName] = append(sheetConfigs[def.ObjName], sheetConfig)
		}
	}

	for buildClassName, configs := range sheetConfigs {
		var buf []byte
		buf, err = gen.Build(buildClassName, configs)
		if err != nil {
			return
		}
		//fmt.Println(string(buf))
		//os.Exit(2)

		buf, err = format.Source(buf)
		if err != nil {
			return
		}

		fileName := tool.ToUnderline(buildClassName)

		filePath := filepath.Join(outputPath, "gen_"+fileName+".go")

		fmt.Println("gen", buildClassName, filePath)
		if err = ioutil.WriteFile(filePath, buf, 0644); err != nil {
			return
		}
	}
	return
}

func parseComment(sheet, comment string) (def *tool.ConfigDefine, sheetConfig *tool.SheetConfig, err error) {
	def = &tool.ConfigDefine{
		Sheet: sheet,
		//T: defType,
		//CfgStruct: customCfgName,
		//Desc: desc,
		//
		//ObjName: buildClassName,
		//
		//Params: params,
	}

	params := strings.Split(comment, tool.Separator)
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
	if builder, ok := tool.ConfigBuilders[def.T]; ok {
		sheetConfig, err = builder.Build(def)
		if err != nil {
			return
		}
	}

	return
}
