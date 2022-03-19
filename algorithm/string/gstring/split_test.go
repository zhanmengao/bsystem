package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// SplitN 以 sep 为分隔符，将 s 切分成多个子串，结果中不包含 sep 本身
// 如果 sep 为空，则将 s 切分成 Unicode 字符列表。
// 如果 s 中没有 sep 子串，则将整个 s 作为 []string 的第一个元素返回
// 参数 n 表示最多切分出几个子串，超出的部分将不再切分。
// 如果 n 为 0，则返回 nil，如果 n 小于 0，则不限制切分个数，全部切分
//func SplitN(s, sep string, n int) []string

func TestSplitN(t *testing.T) {
	s := "Hello, 世界! Hello!"
	ss := strings.SplitN(s, " ", 2)
	fmt.Printf("%q\n", ss) // ["Hello," "世界! Hello!"]
	ss = strings.SplitN(s, " ", -1)
	fmt.Printf("%q\n", ss) // ["Hello," "世界!" "Hello!"]
	ss = strings.SplitN(s, "", 3)
	fmt.Printf("%q\n", ss) // ["H" "e" "llo, 世界! Hello!"]
}

// SplitAfterN 以 sep 为分隔符，将 s 切分成多个子串，结果中包含 sep 本身
// 如果 sep 为空，则将 s 切分成 Unicode 字符列表。
// 如果 s 中没有 sep 子串，则将整个 s 作为 []string 的第一个元素返回
// 参数 n 表示最多切分出几个子串，超出的部分将不再切分。
// 如果 n 为 0，则返回 nil，如果 n 小于 0，则不限制切分个数，全部切分
//func SplitAfterN(s, sep string, n int) []string

func TestSplitAfterN(t *testing.T) {
	s := "Hello, 世界! Hello!"
	ss := strings.SplitAfterN(s, " ", 2)
	fmt.Printf("%q\n", ss) // ["Hello, " "世界! Hello!"]
	ss = strings.SplitAfterN(s, " ", -1)
	fmt.Printf("%q\n", ss) // ["Hello, " "世界! " "Hello!"]
	ss = strings.SplitAfterN(s, "", 3)
	fmt.Printf("%q\n", ss) // ["H" "e" "llo, 世界! Hello!"]
}

// Split 以 sep 为分隔符，将 s 切分成多个子切片，结果中不包含 sep 本身
// 如果 sep 为空，则将 s 切分成 Unicode 字符列表。
// 如果 s 中没有 sep 子串，则将整个 s 作为 []string 的第一个元素返回
//func Split(s, sep string) []string

func TestSplit(t *testing.T) {
	s := "Hello, 世界! Hello!"
	ss := strings.Split(s, " ")
	fmt.Printf("%q\n", ss) // ["Hello," "世界!" "Hello!"]
	ss = strings.Split(s, ", ")
	fmt.Printf("%q\n", ss) // ["Hello" "世界! Hello!"]
	ss = strings.Split(s, "")
	fmt.Printf("%q\n", ss) // 单个字符列表
}

// SplitAfter 以 sep 为分隔符，将 s 切分成多个子切片，结果中包含 sep 本身
// 如果 sep 为空，则将 s 切分成 Unicode 字符列表。
// 如果 s 中没有 sep 子串，则将整个 s 作为 []string 的第一个元素返回
//func SplitAfter(s, sep string) []string

func TestSplitAfter(t *testing.T) {
	s := "Hello, 世界! Hello!"
	ss := strings.SplitAfter(s, " ")
	fmt.Printf("%q\n", ss) // ["Hello, " "世界! " "Hello!"]
	ss = strings.SplitAfter(s, ", ")
	fmt.Printf("%q\n", ss) // ["Hello, " "世界! Hello!"]
	ss = strings.SplitAfter(s, "")
	fmt.Printf("%q\n", ss) // 单个字符列表
}
