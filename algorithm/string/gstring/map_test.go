package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Map 将 s 中满足 mapping(rune) 的字符替换为 mapping(rune) 的返回值。
// 如果 mapping(rune) 返回负数，则相应的字符将被删除。
//func Map(mapping func(rune) rune, s string) string

func Slash(r rune) rune {
	if r == '\\' {
		return '/'
	}
	return r
}
func TestMap(t *testing.T) {
	s := "C:\\Windows\\System32\\FileName"
	ms := strings.Map(Slash, s)
	fmt.Printf("%q\n", ms) // "C:/Windows/System32/FileName"
}
