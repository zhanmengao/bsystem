package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Fields 以连续的空白字符为分隔符，将 s 切分成多个子串，结果中不包含空白字符本身
// 空白字符有：\t, \n, \v, \f, \r, ' ', U+0085 (NEL), U+00A0 (NBSP)
// 如果 s 中只包含空白字符，则返回一个空列表
//func Fields(s string) []string

func TestFields(t *testing.T) {
	s := "Hello, 世界! Hello!"
	ss := strings.Fields(s)
	fmt.Printf("%q\n", ss) // ["Hello," "世界!" "Hello!"]
}

// FieldsFunc 以一个或多个满足 f(rune) 的字符为分隔符，
// 将 s 切分成多个子串，结果中不包含分隔符本身。
// 如果 s 中没有满足 f(rune) 的字符，则返回一个空列表。
//func FieldsFunc(s string, f func(rune) bool) []string

func isSlash(r rune) bool {
	return r == '\\' || r == '/'
}

func TestFieldsFunc(t *testing.T) {
	s := "C:\\Windows\\System32\\FileName"
	ss := strings.FieldsFunc(s, isSlash)
	fmt.Printf("%q\n", ss) // ["C:" "Windows" "System32" "FileName"]
}
