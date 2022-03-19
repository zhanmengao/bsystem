package Function

import (
	"fmt"
	"testing"
)

func Test_Panic(t *testing.T) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Println(r)
		}
	}()

	panic([]int{12312})
}
