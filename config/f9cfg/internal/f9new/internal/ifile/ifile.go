package ifile

import (
	"io/fs"
	"strings"
)

type IFile interface {
	Generate(emb fs.FS, root string, outputDir string, d fs.DirEntry, filePath string, info interface{}) error
}

func GetFileGenerate(fileExt string) IFile {
	if strings.Contains(fileExt, "tpl") {
		return &templateFile{}
	} else {
		return &goFile{}
	}
}
