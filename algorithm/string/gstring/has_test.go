package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// HasPrefix 判断字符串 s 是否以 prefix 开头
//func HasPrefix(s, prefix string) bool

func TestHasPrefix(t *testing.T) {
	s := "Hello 世界!"
	b := strings.HasPrefix(s, "hello")
	fmt.Println(b) // false
	b = strings.HasPrefix(s, "Hello")
	fmt.Println(b) // true
}

// HasSuffix 判断字符串 s 是否以 prefix 结尾
//func HasSuffix(s, suffix string) bool

func TestHasSuffix(t *testing.T) {
	s := "Hello 世界!"
	b := strings.HasSuffix(s, "世界")
	fmt.Println(b) // false
	b = strings.HasSuffix(s, "世界!")
	fmt.Println(b) // true
}
