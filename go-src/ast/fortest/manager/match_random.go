package fortest

import (
	"math/rand"
	"strconv"
	"strings"
	"time"
)

type Random struct {
	begin int
	end   int
	count int
}

func NewRandom(val string) *Random {
	m := &Random{}
	val = strings.ReplaceAll(val, "random:", "")
	ss := strings.Split(val, "-")
	s2 := strings.Split(ss[1], ",")

	m.begin, _ = strconv.Atoi(ss[0])
	m.end, _ = strconv.Atoi(s2[0])
	m.count, _ = strconv.Atoi(s2[1])
	return m
}

func (m *Random) GetMaybeList() []string {
	ret := make([]string, 0)
	for i := 0; i <= m.count; i++ {
		rand.Seed(time.Now().UnixNano())
		ret = append(ret, strconv.Itoa((rand.Int() + i)%(m.end+ 1)+m.begin ))
	}
	return ret
}
