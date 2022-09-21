package fsx

import (
	"embed"
	"path"
	"path/filepath"
	"text/template"
)

func LoadTpl(fs embed.FS, tplFile string) (tpl *template.Template, err error) {
	data, err := fs.ReadFile(path.Join("template", tplFile))
	if err != nil {
		return
	}
	tplName := filepath.Base(tplFile)
	ext := filepath.Ext(tplName)
	tplName = tplName[:len(tplName)-len(ext)-1]
	tpl, err = template.New(tplName).Parse(string(data))
	if err != nil {
		return
	}
	return
}

func LoadManager(fs embed.FS) (fileMap map[string][]byte, err error) {
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
