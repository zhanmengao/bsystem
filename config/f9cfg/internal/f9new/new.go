package f9new

import (
	"bytes"
	"config/f9cfg/internal/f9new/internal/ifile"
	"io/fs"
	"io/ioutil"
	"os"
	"path"
	"strings"
	tp "text/template"
)

func GenFile(emb fs.FS, root string, outputDir string, info interface{}) {
	if err := fs.WalkDir(emb, root, func(filePath string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		ext := path.Ext(d.Name())
		err = ifile.GetFileGenerate(ext).Generate(emb, root, outputDir, d, filePath, info)
		return err
	}); err != nil {
		panic(err)
	}
}

// GenServerFile 直接把server作为根目录，拷贝所有文件
func GenServerFile(emb fs.FS, root string, outputDir string) {
	if err := fs.WalkDir(emb, root, func(goFilePath string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}

		if d.IsDir() {
			goFilePath = strings.ReplaceAll(goFilePath, root, "")
			if strings.HasPrefix(goFilePath, "/") {
				goFilePath = goFilePath[1:]
			}
			if goFilePath != "" {
				err = os.MkdirAll(path.Join(outputDir, goFilePath), 0755)
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
	}); err != nil {
		panic(err)
	}
}

func GenTemplate(emb fs.FS, root string, info interface{}, outputDir string) {
	if err := fs.WalkDir(emb, root, func(tplPath string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if d.IsDir() {
			tplPath = strings.ReplaceAll(tplPath, root, "")
			if strings.HasPrefix(tplPath, "/") {
				tplPath = tplPath[1:]
			}
			if tplPath != "" {
				err = os.MkdirAll(path.Join(outputDir, tplPath), 0755)
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
	}); err != nil {
		panic(err)
	}
}
