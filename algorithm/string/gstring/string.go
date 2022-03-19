package gstring

import (
	"strings"
)

type stdstr string
type strlist []string

//函数指针做参数，将mode替换为f的返回值
func(str *stdstr)Expand(mode string,f func(string)string)string{
	s := strings.Replace(string(*str),mode,f(mode),len(string(*str)))
	return s
}

func(sl *strlist)Join(sp string)string{
	return strings.Join([]string(*sl),sp)
}

func Join(sep string,a ...string)string{
	switch len(a) {
	case 0:
		return ""
	case 1:
		return a[0]
	}
	n := len(sep) * (len(a) - 1)
	for i := 0; i < len(a); i++ {
		n += len(a[i])
	}

	var b strings.Builder
	b.Grow(n)
	b.WriteString(a[0])
	for _, s := range a[1:] {
		b.WriteString(sep)
		b.WriteString(s)
	}
	return b.String()
}
