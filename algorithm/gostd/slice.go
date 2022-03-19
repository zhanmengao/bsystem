package arr

import (
	"fmt"
	//"unicode"
)

func Revert(arr []int) {
	for i, j := 0, len(arr)-1; i < j; i, j = i+1, j-1 {
		arr[i], arr[j] = arr[j], arr[i]
	}
}
func RevertArr(arr [5]int) {
	for i, j := 0, len(arr)-1; i < j; i, j = i+1, j-1 {
		arr[i], arr[j] = arr[j], arr[i]
	}
}
func ClearEmpty(ss []string) []string {
	var i int = 0
	for _, s := range ss {
		if s != "" {
			ss[i] = s
			i++
		}
	}
	ss = ss[:i]
	return ss
}
func RemoveRepeat(ss []string) []string {
	var index int = 0
	for i := 0; i < len(ss)-1; i++ {
		if ss[i] != ss[i+1] {
			ss[index] = ss[i]
			index++
		}
	}
	ss = ss[:index]
	return ss
}
func PrintArray(arr []int) {
	for i := 0; i < len(arr); i++ {
		fmt.Printf("arr[%d] = %d \n", i, arr[i])
	}
}
func PrintSs(arr []string) {
	for i := 0; i < len(arr); i++ {
		fmt.Printf("arr[%d] = %s \n", i, arr[i])
	}
}

func Define() {
	var ss []string
	fmt.Printf("length:%v \taddr:%p \tisnil:%v", len(ss), ss, ss == nil)

	ss = make([]string, 10)
	ss = append(ss, "last")
	print("after append", ss)
}
func CUD() {
	var ss []string
	fmt.Printf("[ local print ]\t:\t length:%v\taddr:%p\tisnil:%v\n", len(ss), ss, ss == nil)
	print("func print", ss)

	//切片尾部追加元素append elemnt
	for i := 0; i < 10; i++ {
		ss = append(ss, fmt.Sprintf("s%d", i))
	}
	fmt.Printf("[ local print ]\t:\tlength:%v\taddr:%p\tisnil:%v\n", len(ss), ss, ss == nil)
	print("after append", ss)

	//删除切片元素remove element at index
	index := 5
	ss = append(ss[:index], ss[index+1:]...)
	print("after delete", ss)

	//在切片中间插入元素insert element at index;
	//注意：保存后部剩余元素，必须新建一个临时切片
	rear := append([]string{}, ss[index:]...)
	ss = append(ss[0:index], "inserted")
	ss = append(ss, rear...)
	print("after insert", ss)
}

func Copy() {
	var sa = make([]string, 0)
	for i := 0; i < 10; i++ {
		sa = append(sa, fmt.Sprintf("%v", i))

	}
	var da = make([]string, 0, 10)
	var cc = 0
	cc = copy(da, sa)
	fmt.Printf("copy to da(len=%d)\t%v\n", len(da), da)
	da = make([]string, 5)
	cc = copy(da, sa)
	fmt.Printf("copy to da(len=%d)\tcopied=%d\t%v\n", len(da), cc, da)
	da = make([]string, 10)
	cc = copy(da, sa)
	fmt.Printf("copy to da(len=%d)\tcopied=%d\t%v\n", len(da), cc, da)
}

func Point() {
	var sa []string
	fmt.Printf("addr:%p \t\tlen:%v content:%v\n", sa, len(sa), sa)
	for i := 0; i < 10; i++ {
		sa = append(sa, fmt.Sprintf("%v", i))
		fmt.Printf("addr:%p \t\tlen:%v content:%v\n", sa, len(sa), sa)
	}
	fmt.Printf("addr:%p \t\tlen:%v content:%v\n", sa, len(sa), sa)
}

type UserList struct {
	list []int
}

func CopyLevel() UserList {
	var u UserList
	u.list = make([]int, 100)
	fmt.Printf("u addr %p  \t\t list addr %p \n", &u, u.list)
	return u
}
