package generator

import (
	"bytes"
	"config/f9cfg/internal/tabletype"
	"fmt"
	"html/template"
	"strings"
)

var (
	tplBaseDir   = "templates/"
	fileTpl      = "file.tpl"
	funcsBaseTpl = []string{}
	funcsTplMap  = map[string]string{
		tabletype.TypeStruct:       "struct.tpl",
		tabletype.TypeMap:          "map.tpl",
		tabletype.TypeList:         "list.tpl",
		tabletype.TypeGroup:        "group.tpl",
		tabletype.TypeGroupMap:     "group_map.tpl",
		tabletype.TypeGroupMapList: "group_map_list.tpl",
	}
)

type SheetTplData struct {
	Cfg             *tabletype.SheetConfig
	BuildClassName  string
	ProtoPackage    string
	CfgDefine       string
	BuildStructName string

	LoadFuncs []*LoadFunc
}

type LoadFunc struct {
	SheetName   string
	FuncName    string
	FuncBody    string
	SheetConfig *tabletype.SheetConfig
}

type FuncGenerator struct {
	tmpl map[string]*template.Template
}

func (fg *FuncGenerator) Init() (err error) {
	tmpl := make(map[string]*template.Template)

	for typ, tplsName := range funcsTplMap {
		if tmpl[typ], err = loadTPL(tplsName); err != nil {
			panic(err)
		}
	}

	fg.tmpl = tmpl

	return
}

func (fg *FuncGenerator) Build(config *SheetTplData) (funcStr string, err error) {
	t, ok := fg.tmpl[config.Cfg.VarType]
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

func NewGenerator() (g *FileGenerator, err error) {
	funcGen := new(FuncGenerator)

	g = &FileGenerator{}

	if g.tmpl, err = loadBaseTPL(fileTpl); err != nil {
		return
	}

	if err = funcGen.Init(); err != nil {
		panic(err)
	}

	g.funcGen = funcGen
	return
}

func (g *FileGenerator) Build(buildClassName string, ProtoPackage string, sheetCfg *tabletype.SheetConfig) (buff []byte, err error) {
	sheetData := &SheetTplData{
		BuildClassName:  buildClassName,
		Cfg:             sheetCfg,
		ProtoPackage:    ProtoPackage,
		CfgDefine:       sheetCfg.CfgDefine,
		BuildStructName: strings.ToLower(buildClassName),
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

// filterSpecChar 过滤掉特殊字符
func (g *FileGenerator) filterSpecChar(funcName string) string {
	return strings.Replace(funcName, ".", "", -1)
}

func (g *FileGenerator) loadFuncs(sheetData *SheetTplData) (err error) {
	funcs := make([]*LoadFunc, 0, 1)
	lc := &LoadFunc{
		SheetName:   sheetData.Cfg.SheetVar,
		SheetConfig: sheetData.Cfg,
	}

	lc.FuncBody, err = g.genFuncBody(sheetData)
	if err != nil {
		fmt.Println("gen func body err", err.Error(), sheetData.Cfg)
		return
	}
	funcs = append(funcs, lc)
	sheetData.LoadFuncs = funcs

	return
}

func (g *FileGenerator) genFuncBody(config *SheetTplData) (body string, err error) {
	funcStr, err := g.funcGen.Build(config)
	if err != nil {
		return
	}

	body = funcStr

	return
}

func (g *FileGenerator) unescaped(x string) interface{} {
	return template.HTML(x)
}
