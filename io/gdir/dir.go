package main

import (
	"fmt"
	"io/ioutil"
	"os"
)

//获取指定目录下的所有文件和目录

func ListDir(dirPth string) (files []string, dirs []string, err error) {
	//fmt.Println(dirPth)
	dir, err := ioutil.ReadDir(dirPth)
	if err != nil {
		return nil, nil, err
	}
	PthSep := string(os.PathSeparator)
	//    suffix = strings.ToUpper(suffix) //忽略后缀匹配的大小写

	for _, fi := range dir {
		if fi.IsDir() {
			dirs = append(dirs, dirPth+PthSep+fi.Name())
			ListDir(dirPth + PthSep + fi.Name())

		} else {
			files = append(files, dirPth+PthSep+fi.Name())
		}
	}
	return files, dirs, nil

}

func main() {
	path, _ := os.Getwd()
	files, dirs, _ := ListDir(path)
	for _, table := range dirs {
		temp, _, _ := ListDir(table)
		for _, temp1 := range temp {
			files = append(files, temp1)
		}
	}
	for _, table1 := range files {
		fmt.Println("files : ",table1)
	}
	for _, dir := range dirs {
		fmt.Println("dir : ",dir)
	}
}
