package gstring

import (
	"strconv"
	"testing"
)
import "fmt"

func TestConvert(t *testing.T) {
	str := "2731231313111600"
	i, _ := strconv.ParseInt(str, 10, 64)
	//int64转成string：
	s := strconv.FormatInt(i, 10)
	fmt.Printf("%v : %s \n", i, s)

	str = "0"
	i, _ = strconv.ParseInt(str, 10, 64)
	fmt.Printf("%v \n", i)
}
