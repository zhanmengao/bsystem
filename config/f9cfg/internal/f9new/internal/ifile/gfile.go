package ifile

import (
	"io/fs"
	"io/ioutil"
	"os"
	"path"
	"strings"
)

type goFile struct {
}

func (f *goFile) Generate(emb fs.FS, root string, outputDir string, d fs.DirEntry, goFilePath string, info interface{}) error {
	if d.IsDir() {
		goFilePath = strings.ReplaceAll(goFilePath, root, "")
		if strings.HasPrefix(goFilePath, "/") {
			goFilePath = goFilePath[1:]
		}
		if goFilePath != "" {
			err := os.MkdirAll(path.Join(outputDir, goFilePath), 0755)
			return err
		}
	} else {
		body, err := fs.ReadFile(emb, goFilePath)
		if err != nil {
			return err
		}
		goFilePath = strings.ReplaceAll(goFilePath, root, "")
		//拷贝文件放到指定目录
		if err = ioutil.WriteFile(path.Join(outputDir, goFilePath), body, 0644); err != nil {
			return err
		}
	}

	return nil
}
