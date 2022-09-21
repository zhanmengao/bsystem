package ifile

import (
	"bytes"
	"io/fs"
	"io/ioutil"
	"os"
	"path"
	"strings"
	tp "text/template"
)

type templateFile struct {
}

func (f *templateFile) Generate(emb fs.FS, root string, outputDir string, d fs.DirEntry, tplPath string, info interface{}) error {
	if d.IsDir() {
		tplPath = strings.ReplaceAll(tplPath, root, "")
		if strings.HasPrefix(tplPath, "/") {
			tplPath = tplPath[1:]
		}
		if tplPath != "" {
			err := os.MkdirAll(path.Join(outputDir, tplPath), 0755)
			return err
		}
	} else {
		buf := new(bytes.Buffer)
		t, err := tp.ParseFS(emb, tplPath)
		if err != nil {
			panic(err)
		}
		if err = t.Execute(buf, info); err != nil {
			panic(err)
		}
		goFile := strings.ReplaceAll(tplPath, ".tpl", "")
		goFile = strings.ReplaceAll(goFile, root, "")
		//拷贝文件放到指定目录
		if err = ioutil.WriteFile(path.Join(outputDir, goFile), buf.Bytes(), 0644); err != nil {
			return err
		}
	}

	return nil
}
