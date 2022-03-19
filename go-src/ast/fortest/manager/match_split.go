package fortest

import (
	"strings"
)

type Split struct {
	val string
}

func NewSplit(val string) *Split {
	return &Split{
		val: val,
	}
}

func (m *Split) GetMaybeList() []string {
	ss := strings.Split(m.val, ",")
	if len(ss) <= 0 {
		return []string{m.val}
	}
	ret := make([]string, 0, len(ss))
	for _, v := range ss {
		if v != "" {
			ret = append(ret, v)
		}
	}
	return ret
}
