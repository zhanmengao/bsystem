package gstring

import (
	"fmt"
	"testing"
)

func TestString(t *testing.T) {
	var s stdstr
	s = "aabbccccaaa"
	s = stdstr(s.Expand("aa",func(s string)string{			//lambda
		return "xx"
	}))
	fmt.Println(s)

	ss := []string{"aa","bb","cc"}
	sl := strlist(ss)
	sl.Join("+")
	fmt.Println(sl)
}
