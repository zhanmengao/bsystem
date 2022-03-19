package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Index 返回子串 sep 在字符串 s 中第一次出现的位置
// 如果找不到，则返回 -1，如果 sep 为空，则返回 0。
// 使用 Rabin-Karp 算法实现
//func Index(s, sep string) int

func TestIndex(t *testing.T) {
	s := "Hello,世界!"
	i := strings.Index(s, "h")
	fmt.Println(i) // -1
	i = strings.Index(s, "!")
	fmt.Println(i) // 12
	i = strings.Index(s, "")
	fmt.Println(i) // 0
}

// LastIndex 返回子串 sep 在字符串 s 中最后一次出现的位置
// 如果找不到，则返回 -1，如果 sep 为空，则返回字符串的长度
// 使用朴素字符串比较算法实现
//func LastIndex(s, sep string) int

func TestLastIndex(t *testing.T) {
	s := "Hello,世界! Hello!"
	i := strings.LastIndex(s, "h")
	fmt.Println(i) // -1
	i = strings.LastIndex(s, "H")
	fmt.Println(i) // 14
	i = strings.LastIndex(s, "")
	fmt.Println(i) // 20
}

// IndexRune 返回字符 r 在字符串 s 中第一次出现的位置
// 如果找不到，则返回 -1
//func IndexRune(s string, r rune) int

func TestIndexRune(t *testing.T) {
	s := "Hello,世界! Hello!"
	i := strings.IndexRune(s, '\n')
	fmt.Println(i) // -1
	i = strings.IndexRune(s, '界')
	fmt.Println(i) // 9
	i = strings.IndexRune(s, 0)
	fmt.Println(i) // -1
}


// IndexAny 返回字符串 chars 中的任何一个字符在字符串 s 中第一次出现的位置
// 如果找不到，则返回 -1，如果 chars 为空，则返回 -1
//func IndexAny(s, chars string) int

func TestIndexAny(t *testing.T) {
	s := "Hello,世界! Hello!"
	i := strings.IndexAny(s, "abc")
	fmt.Println(i) // -1
	i = strings.IndexAny(s, "dof")
	fmt.Println(i) // 1
	i = strings.IndexAny(s, "")
	fmt.Println(i) // -1
}


// LastIndexAny 返回字符串 chars 中的任何一个字符在字符串 s 中最后一次出现的位置
// 如果找不到，则返回 -1，如果 chars 为空，也返回 -1
//func LastIndexAny(s, chars string) int
func TestLastIndexAny(t *testing.T) {
	s := "Hello,世界! Hello!"
	i := strings.LastIndexAny(s, "abc")
	fmt.Println(i) // -1
	i = strings.LastIndexAny(s, "def")
	fmt.Println(i) // 15
	i = strings.LastIndexAny(s, "")
	fmt.Println(i) // -1
}

// 返回 s 中第一个满足 f(rune) 的字符的字节位置。
// 如果没有满足 f(rune) 的字符，则返回 -1
//func IndexFunc(s string, f func(rune) bool) int

func TestIndexFunc(t *testing.T) {
	s := "C:\\Windows\\System32"
	i := strings.IndexFunc(s, isSlash)
	fmt.Printf("%v\n", i) // 2
}