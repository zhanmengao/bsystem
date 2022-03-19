package arr

import (
	"fmt"
	"testing"
	"unsafe"
)

type slice struct {
	array unsafe.Pointer
	len   int
	cap   int
}

func TestSlice(t *testing.T) {
	var arr []int = []int{0, 1, 2, 3, 5}
	Revert(arr)
	PrintArray(arr)
	var ss []string = []string{"aa", "bbb", "", "ccc", "ccc", "ccc"}
	ss = ClearEmpty(ss)
	PrintSs(ss)
	ss = RemoveRepeat(ss)
	PrintSs(ss)
	var sUnicode []string = []string{"aa", "bbb", "澜", "？？", " ", "显卡"}
	PrintSs(sUnicode)
}

func TestCopy(t *testing.T) {
	Point()
	u := CopyLevel()
	fmt.Printf("u addr %p  \t\t list addr %p \n", &u, u.list)
}

func TestCute(t *testing.T) {
	array := []int{10, 20, 30, 40}
	slice := array[0:2]
	for i, v := range slice {
		fmt.Printf("sli[%d] = %d \n", i, v)
	}
	sli2 := array[:len(array)]
	for i, v := range sli2 {
		fmt.Printf("sli[%d] = %d \n", i, v)
	}
	sli3 := array[:5]
	for i, v := range sli3 {
		fmt.Printf("sli[%d] = %d \n", i, v)
	}
}

func Test_notResize(t *testing.T) {
	array := [4]int{10, 20, 30, 40}
	slice := array[0:2]
	fmt.Printf("slice addr = %p \n", slice)
	newSlice := append(slice, 50)
	newSlice[1] += 1
	fmt.Println(slice) //change
	fmt.Printf("newslice addr = %p \n slice addr = %p ", newSlice, slice)
}

func Test_resize(t *testing.T) {
	array := [4]int{10, 20, 30, 40}
	slice := array[0:2]
	newSlice := append(append(append(slice, 50), 100), 150)
	newSlice[1] += 1
	fmt.Println(slice)
	fmt.Printf("newslice addr = %p \n slice addr = %p \n", newSlice, slice)
}

func Test_Range(t *testing.T) {
	array := [4]int{10, 20, 30, 40}
	slice := array[0:2]
	newSlice := append(append(append(slice, 50), 100), 150)
	i := 0
	for i = range newSlice {

	}
	fmt.Printf("sli = %d i = %d \n", len(newSlice), i)
}
