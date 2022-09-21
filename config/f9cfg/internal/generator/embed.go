package generator

import (
	"embed"
	"html/template"
	"path"
	"path/filepath"
)

//go:embed template/*.tpl manager/*.go
var fs embed.FS

func loadBaseTPL(tplFile string) (tpl *template.Template, err error) {
	data, err := fs.ReadFile(path.Join("template", tplFile))
	if err != nil {
		return
	}
	tplName := filepath.Base(tplFile)
	ext := filepath.Ext(tplName)
	tplName = tplName[:len(tplName)-len(ext)]
	tpl, err = template.New(tplName).Funcs(template.FuncMap{
		"unescaped": unescaped,
	}).Parse(string(data))
	if err != nil {
		return
	}
	return
}

func unescaped(x string) interface{} {
	return template.HTML(x)
}

func loadTPL(tplFile string) (tpl *template.Template, err error) {
	tplFile = path.Join("template", tplFile)
	tpl = template.Must(template.ParseFS(fs, tplFile))
	return
}

func loadManager() (fileMap map[string][]byte, err error) {
	fileList, err := fs.ReadDir("manager")
	if err != nil {
		return
	}
	fileMap = make(map[string][]byte, len(fileList))
	for _, v := range fileList {
		var body []byte
		body, err = fs.ReadFile(path.Join("manager", v.Name()))
		if err != nil {
			return
		}
		fileMap[v.Name()] = body
	}
	return
}
