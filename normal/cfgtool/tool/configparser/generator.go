package configparser

import (
	"bytes"
	"fmt"
	"html/template"
	"path/filepath"
	"strings"
)
const (
	TypeStruct       = "Struct"
	TypeMap          = "Map"
	TypeList         = "List"
	TypeGroup        = "Group"
	TypeGroupMap     = "GroupMap"
	TypeGroupMapList = "GroupMapList"
	Separator        = "|"
)
var (
	tplBaseDir   = "templates/"
	fileTpl      = "file.tpl"
	funcsBaseTpl = []string{}
	funcsTplMap  = map[string][]string{
		TypeStruct: {"struct.tpl"},
		TypeMap: {
			"map.tpl",
			//"map-origin.tpl",
			//"map-map.tpl",
			//"map-group.tpl",
			//"group-origin.tpl",
		},
		TypeList:         {"list.tpl"},
		TypeGroup:        {"group.tpl"},
		TypeGroupMap:     {"group_map.tpl"},
		TypeGroupMapList: {"group_map_list.tpl"},
	}
)

type SheetTplData struct {
	BuildClassName      string
	BuildObjName        string
	SheetList           []*SheetConfig
	SheetMapTypeConfigs map[string][]*SheetConfig // key type
	SheetMapSheetName   map[string]*SheetConfig   // key sheetName

	LoadFuncs []*LoadFunc
}

type LoadFunc struct {
	SheetName   string
	FuncName    string
	FuncBody    string
	SheetConfig *SheetConfig
}

type FuncGenerator struct {
	tmpl map[string]*template.Template
}

func (fg *FuncGenerator) Init(tplDir string) (err error) {
	tmpl := make(map[string]*template.Template)

	for typ, tplsName := range funcsTplMap {
		tplsFile := make([]string, 0, 3)

		for _, tn := range tplsName {
			tplsFile = append(tplsFile, tplDir+tn)
		}

		for _, bt := range funcsBaseTpl {
			tplsFile = append(tplsFile, tplDir+bt)
		}

		tmpl[typ] = template.Must(template.ParseFiles(tplsFile...))
	}

	fg.tmpl = tmpl

	return
}

func (fg *FuncGenerator) Build(config *SheetConfig) (funcStr string, err error) {
	t, ok := fg.tmpl[config.VarType]
	if ok {
		buff := bytes.NewBufferString("")
		err = t.Execute(buff, config)
		if err != nil {
			return
		}

		funcStr = buff.String()
	}

	return
}

type FileGenerator struct {
	tmpl    *template.Template
	funcGen *FuncGenerator
}

func NewGenerator(root string) *FileGenerator {
	funcGen := new(FuncGenerator)
	if len(root) != 0 {
		tplBaseDir = filepath.Join(root, tplBaseDir)
	}

	g := &FileGenerator{}

	g.tmpl = template.Must(template.New("file").Funcs(template.FuncMap{
		"unescaped": g.unescaped,
	}).ParseFiles(filepath.Join(tplBaseDir, fileTpl)))

	if err := funcGen.Init(filepath.Join(tplBaseDir, "funcs") + "/"); err != nil {
		panic(err.Error())
	}

	g.funcGen = funcGen
	return g
}

func (g *FileGenerator) Build(buildClassName string, configs []*SheetConfig) (buff []byte, err error) {
	sheetData := &SheetTplData{
		BuildClassName: buildClassName,
		BuildObjName:   ToHump(buildClassName) + "Obj",
		SheetList:      configs,
	}

	for _, config := range sheetData.SheetList {
		config.BuildClassName = sheetData.BuildClassName
		config.BuildObjName = sheetData.BuildObjName
		config.FuncName = g.filterSpecChar(config.FuncName)
		config.KeyLower = g.filterSpecChar(config.KeyLower)
	}

	err = g.fileConfigs(sheetData)
	if err != nil {
		return
	}

	err = g.loadFuncs(sheetData)
	if err != nil {
		return
	}

	b := bytes.NewBufferString("")
	err = g.tmpl.Execute(b, sheetData)
	if err != nil {
		return
	}
	buff = b.Bytes()

	return
}

func (g *FileGenerator) fileConfigs(sheetData *SheetTplData) (err error) {
	sheets := make(map[string]*SheetConfig)

	sheetMapTypeConfigs := make(map[string][]*SheetConfig)

	for _, config := range sheetData.SheetList {
		sheets[config.SheetVar] = config

		if sheetMapTypeConfigs[config.VarType] == nil {
			sheetMapTypeConfigs[config.VarType] = make([]*SheetConfig, 0, 50)
		}

		v := *config
		// 获取真实的值类型
		//v.ValueType = g.getRealValueType(v)

		sheetMapTypeConfigs[config.VarType] = append(sheetMapTypeConfigs[config.VarType], &v)

	}

	sheetData.SheetMapSheetName = sheets
	sheetData.SheetMapTypeConfigs = sheetMapTypeConfigs

	return
}

//func (g *FileGenerator) getRealValueType(v SheetConfig) string {
//	if strings.Contains(v.ValueType, Mark) {
//		switch v.ValueType {
//		case "@map":
//			v.ValueType = fmt.Sprintf("map[%s]%s", v.ComplexSheet.KeyType,
//				v.ComplexSheet.ValueType)
//		case "@group":
//			v.ValueType = fmt.Sprintf("map[%s][]%s", v.ComplexSheet.KeyType,
//				v.ComplexSheet.ValueType)
//		}
//	}
//
//	return v.ValueType
//}

// filterSpecChar 过滤掉特殊字符
func (g *FileGenerator) filterSpecChar(funcName string) string {
	return strings.Replace(funcName, ".", "", -1)
}

func (g *FileGenerator) loadFuncs(sheetData *SheetTplData) (err error) {
	funcs := make([]*LoadFunc, 0, len(sheetData.SheetList))

	for _, config := range sheetData.SheetList {
		lc := &LoadFunc{
			SheetName:   config.SheetVar,
			FuncName:    config.FuncName,
			SheetConfig: config,
		}

		lc.FuncBody, err = g.genFuncBody(config)
		if err != nil {
			fmt.Println("gen func body err", err.Error(), config)
			return
		}

		funcs = append(funcs, lc)
	}

	sheetData.LoadFuncs = funcs

	return
}

func (g *FileGenerator) genFuncBody(config *SheetConfig) (body string, err error) {
	funcStr, err := g.funcGen.Build(config)
	if err != nil {
		return
	}

	body = string(funcStr)

	return
}

func (g *FileGenerator) unescaped(x string) interface{} {
	return template.HTML(x)
}
