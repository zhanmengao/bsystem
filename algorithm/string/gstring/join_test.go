package gstring

import (
	"fmt"
	"strings"
	"testing"
)

// Join 将 a 中的子串连接成一个单独的字符串，子串之间用 sep 分隔
//func Join(a []string, sep string) string

func TestJoin(t *testing.T) {
	ss := []string{"Monday", "Tuesday", "Wednesday"}
	s := strings.Join(ss, "|")
	fmt.Println(s)
}
