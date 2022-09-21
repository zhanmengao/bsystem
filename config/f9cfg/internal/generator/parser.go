package generator

import (
	"config/f9cfg/internal/meta"
	"config/f9cfg/internal/tabletype"
	"fmt"
	"go/ast"
	"go/format"
	"io/ioutil"
	"path/filepath"
	"strings"
)

// 解析文件所有路径

type CustomParser struct {
	msgStart    bool
	msgComments string
	PbPackage   string

	comments map[string]string
	typeInfo map[string]*ast.TypeSpec
}

func NewParser() *CustomParser {
	return &CustomParser{
		comments: make(map[string]string, 5),
		typeInfo: make(map[string]*ast.TypeSpec, 5),
	}
}

func (p *CustomParser) BuildLoader(fg *FileGenerator, ProtoPackage, pkgName string, outputPath string, comments map[string]string, typeDefine map[string]*ast.TypeSpec) (err error) {
	sheetConfigs := make(map[string]*tabletype.SheetConfig)
	for aryName, comment := range comments {
		// 解析注释

		tp := typeDefine[aryName]
		var filterType string
		for _, v := range tp.Type.(*ast.StructType).Fields.List {
			ft, fn := meta.GetFieldName(v, ProtoPackage)
			if fn == "Ary" {
				filterType = ft
				break
			}
		}
		//从Ary里找到KeyType
		def := tabletype.NewConfigDefine(aryName, comment, pkgName, typeDefine[filterType])
		var sheetConfig *tabletype.SheetConfig
		sheetConfig, err = def.ParseComment()
		if err != nil {
			return
		}
		sheetConfig.CfgDefine += strings.Replace(comment, "//", "", 1)

		sheetConfigs[def.Sheet] = sheetConfig
	}

	for buildClassName, configs := range sheetConfigs {
		var buf []byte
		buf, err = fg.Build(buildClassName, ProtoPackage, configs)
		if err != nil {
			return
		}
		cpy := make([]byte, len(buf))
		copy(cpy, buf)
		fileName := tabletype.ToUnderline(buildClassName)

		filePath := filepath.Join(outputPath, fileName+".cfg.go")
		buf, err = format.Source(buf)

		if err != nil {
			if err2 := ioutil.WriteFile(filePath, cpy, 0644); err2 != nil {
				return
			}
			fmt.Printf("code : %s \n", cpy)
			return
		}
		fmt.Println("gen", buildClassName, filePath)
		if err = ioutil.WriteFile(filePath, buf, 0644); err != nil {
			return
		}
	}
	return
}
