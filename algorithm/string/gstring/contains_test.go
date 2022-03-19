package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Contains 判断字符串 s 中是否包含子串 substr
// 如果 substr 为空，则返回 true
//func Contains(s, substr string) bool

func TestContains(t *testing.T) {
	s := "Hello,世界!!!!!"
	b := strings.Contains(s, "!!")
	fmt.Println(b) // true
	b = strings.Contains(s, "!?")
	fmt.Println(b) // false
	b = strings.Contains(s, "")
	fmt.Println(b) // true
}

// ContainsAny 判断字符串 s 中是否包含 chars 中的任何一个字符
// 如果 chars 为空，则返回 false
//func ContainsAny(s, chars string) bool
func TestContainsAny(t *testing.T) {
	s := "Hello,世界!"
	b := strings.ContainsAny(s, "abc")
	fmt.Println(b) // false
	b = strings.ContainsAny(s, "def")
	fmt.Println(b) // true
	b = strings.Contains(s, "")
	fmt.Println(b) // true
}


// ContainsRune 判断字符串 s 中是否包含字符 r
//func ContainsRune(s string, r rune) bool
func TestContainsRune(t *testing.T) {
	s := "Hello,世界!"
	b := strings.ContainsRune(s, '\n')
	fmt.Println(b) // false
	b = strings.ContainsRune(s, '界')
	fmt.Println(b) // true
	b = strings.ContainsRune(s, 0)
	fmt.Println(b) // false
}