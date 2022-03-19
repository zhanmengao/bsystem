package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Title 将 s 中的所有单词的首字母修改为其 Title 格式
// BUG: Title 规则不能正确处理 Unicode 标点符号
//func Title(s string) string

func TestTitle(t *testing.T) {
	s := "heLLo worLd"
	ts := strings.Title(s)
	fmt.Printf("%q\n", ts) // "HeLLo WorLd"
}