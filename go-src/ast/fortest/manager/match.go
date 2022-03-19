package fortest

import (
	"strconv"
	"strings"
)

func getMatch(v string) IMatch {
	if strings.Contains(v, "random") {
		return NewRandom(v)
	} else if strings.Contains(v, "mysql") {
		return NewMysqlMatch(v)
	} else if strings.Contains(v,"redis"){
		return NewRedisMatch(v)
	} else{
		return NewSplit(v)
	}
}


func getVal(val interface{}) (ret string) {
	switch val.(type) {
	case *int:
		ret = strconv.FormatInt(int64(*val.(*int)), 10)
	case *string:
		ret = *val.(*string)
	case *float64:
		ret = strconv.FormatFloat(*val.(*float64), 'f', 2, 64)
	}
	return
}