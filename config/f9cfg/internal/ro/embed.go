package ro

import (
	"embed"
	"path/filepath"
	"text/template"
)

//go:embed template/*.tpl
var fs embed.FS


func loadTpl(tplFile string) (tpl *template.Template, err error) {
	data, err := fs.ReadFile(tplFile)
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