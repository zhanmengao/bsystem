package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Repeat 将 count 个字符串 s 连接成一个新的字符串
//func Repeat(s string, count int) string

func TestRepeat(t *testing.T) {
	s := "Hello!"
	rs := strings.Repeat(s, 3)
	fmt.Printf("%q\n", rs) // "Hello!Hello!Hello!"
}
