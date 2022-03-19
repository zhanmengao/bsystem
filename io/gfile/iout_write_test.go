package gio

import (
	"io/ioutil"
	"testing"
)

//这种方式每次都会覆盖 test.txt内容，如果test.txt文件不存在会创建。
func TestWriteFile(t *testing.T) {
	content := []byte("测试1\n测试2\n")
	err := ioutil.WriteFile("test.txt", content, 0644)
	if err != nil {
		panic(err)
	}
}
