package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// TrimLeftFunc 将删除 s 头部连续的满足 f(rune) 的字符
//func TrimLeftFunc(s string, f func(rune) bool) string

// TrimRightFunc 将删除 s 尾部连续的满足 f(rune) 的字符
//func TrimRightFunc(s string, f func(rune) bool) string

func TestTrimRightFunc(t *testing.T) {
	s := "\\\\HostName\\C\\Windows\\"
	ts := strings.TrimRightFunc(s, isSlash)
	fmt.Printf("%q\n", ts) // "\\\\HostName\\C\\Windows"
}

// TrimFunc 将删除 s 首尾连续的满足 f(rune) 的字符
//func TrimFunc(s string, f func(rune) bool) string

func TestTrimFunc(t *testing.T) {
	s := "\\\\HostName\\C\\Windows\\"
	ts := strings.TrimFunc(s, isSlash)
	fmt.Printf("%q\n", ts) // "HostName\\C\\Windows"
}

// Trim 将删除 s 首尾连续的包含在 cutset 中的字符
//func Trim(s string, cutset string) string

func TestTrim(t *testing.T) {
	s := " Hello 世界! "
	ts := strings.Trim(s, " Helo!")
	fmt.Printf("%q\n", ts) // "世界"
}



// TrimLeft 将删除 s 头部连续的包含在 cutset 中的字符
//func TrimLeft(s string, cutset string) string

func TestTrimLeft(t *testing.T) {
	s := " Hello 世界! "
	ts := strings.TrimLeft(s, " Helo")
	fmt.Printf("%q\n", ts) // "世界! "
}

// TrimRight 将删除 s 尾部连续的包含在 cutset 中的字符
//func TrimRight(s string, cutset string) string

func TestTrimRight(t *testing.T) {
	s := " Hello 世界! "
	ts := strings.TrimRight(s, " 世界!")
	fmt.Printf("%q\n", ts) // " Hello"
}

// TrimSpace 将删除 s 首尾连续的的空白字符
//func TrimSpace(s string) string

func TestTrimSpace(t *testing.T) {
	s := " Hello 世界! "
	ts := strings.TrimSpace(s)
	fmt.Printf("%q\n", ts) // "Hello 世界!"
}

// TrimPrefix 删除 s 头部的 prefix 字符串
// 如果 s 不是以 prefix 开头，则返回原始 s
//func TrimPrefix(s, prefix string) string

func TestTrimPrefix(t *testing.T) {
	s := "Hello 世界!"
	ts := strings.TrimPrefix(s, "Hello")
	fmt.Printf("%q\n", ts) // " 世界"
}

// TrimSuffix 删除 s 尾部的 suffix 字符串
// 如果 s 不是以 suffix 结尾，则返回原始 s
//func TrimSuffix(s, suffix string) string

func TestTrimSuffix(t *testing.T) {
	s := "Hello 世界!!!!!"
	ts := strings.TrimSuffix(s, "!!!!")
	fmt.Printf("%q\n", ts) // " 世界"
}
