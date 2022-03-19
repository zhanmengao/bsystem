package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Count 计算字符串 sep 在 s 中的非重叠个数
// 如果 sep 为空字符串，则返回 s 中的字符(非字节)个数 + 1
// 使用 Rabin-Karp 算法实现

//func Count(s, sep string) int

func TestCount(t *testing.T) {
	s := "Hello,世界!!!!!"
	n := strings.Count(s, "!")
	fmt.Println(n) // 5
	n = strings.Count(s, "!!")
	fmt.Println(n) // 2
}
