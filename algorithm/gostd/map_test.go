package arr

import (
	"fmt"
	"testing"
)

func TestMap(t *testing.T) {
	m := make(map[int]string)
	m[1] = "m1"
	m[2] = "m2"

	PrintVal(m, 3)
	PrintVal(m, 2)
	delete(m, 2)
	PrintVal(m, 2)
}


func Test_hget(t *testing.T) {
	newMap := make(map[string]int)
	fmt.Println(newMap["a"])
}

func Test_range(t *testing.T) {
	newMap := make(map[int]int)
	for i := 0; i < 10; i++{
		newMap[i] = i
	}
	for key, value := range newMap{
		fmt.Printf("key is %d, value is %d\n", key, value)
	}
}