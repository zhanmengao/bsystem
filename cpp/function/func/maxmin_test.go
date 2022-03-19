package Function

import (
	"fmt"
	"testing"
)

const INT_MAX = int(^uint(0) >> 1)
const INT_MIN = ^INT_MAX

func max(vals ...int) int {
	var max = INT_MIN
	for _, v := range vals {
		if v > max {
			max = v
		}
	}
	return max
}

func Test_MinMax(t *testing.T) {
	values := []int{1, 2, 3, 4, 5, 6, 7}
	num := max(values...)
	fmt.Printf("max = %d", num)
}
