package class

import (
	"fmt"
	"testing"
)

type User struct {
	name string
}

func (u User) SetNameValueType(str string) {
	fmt.Printf("SetNameValueType() pointer:%p\n", &u) // SetNameValueType() pointer:0xc000096240
	u.name = str
}

func (u *User) SetNamePointerType(str string) {
	fmt.Printf("SetNamePointerType() pointer:%p\n", u) // SetNamePointerType() pointer:0xc000096220
	u.name = str
}

func TestRecv(t *testing.T) {
	user1 := &User{}
	fmt.Printf("pointer:%p\n", user1) 		// pointer:0xc000096220
	fmt.Println(user1) 								// &{}

	user1.SetNameValueType("lucy")
	fmt.Println(user1) 								// &{}

	user1.SetNamePointerType("lily")
	fmt.Println(user1) 								// &{lily}
}
/*
阅读上面这段代码，我们可以发现值类型的接收者，调用方拷贝了副本；指针类型的接收者，调用方未拷贝副本。
 */

func TestSlice(t *testing.T) {
	user1 := &User{}
	fmt.Printf("pointer:%p\n", user1) // pointer:0xc000096220
	fmt.Println(user1) 								// &{}
	user1.SetNameValueType("lucy")
	fmt.Println(user1) 								// &{}
	user1.SetNamePointerType("lily")
	fmt.Println(user1) 								// &{lily}

	// m := make(map[int]int)
	m := map[int]int{}
	fmt.Printf("map pointer:%p\n", m) 		// map pointer:0xc000100180
	m[0] = 1
	fmt.Printf("map pointer:%p\n", m) 		// map pointer:0xc000100180
	m[1] = 2

	s := make([]int, 0, 1)
	fmt.Printf("slice pointer:%p\n", s) 			// slice pointer:0xc00001c0a0
	s = append(s, 1)
	fmt.Printf("slice pointer:%p\n", s) 			// slice pointer:0xc00001c0a0
	s = append(s, 2)
	fmt.Printf("slice pointer:%p\n", s) 			// slice pointer:0xc00001c0b0
}
/*
阅读上面这段代码，我们可以发现 map 类型未分配新内存地址，使用 append 函数向 slice 中追加元素，当元素个数未超出其容量之前，slice 也未分配新内存地址。
 */

func (u User) ValueSetName(str string) User {
	u.name = str
	return u
}

func Test3(t *testing.T) {
	user2 := &User{}
	fmt.Printf("user2 pointer:%p\n", user2) 						// user2 pointer:0xc000010290
	user2.SetNameValueType("tom") 									// SetNameValueType() pointer:0xc0000102a0

	user3 := &User{}
	fmt.Printf("user3 pointer:%p\n", user3) 						// user3 pointer:0xc0000102b0
	user3.ValueSetName("bob")
	fmt.Printf("pointer:%p\n", user3) 							// pointer:0xc0000102b0
}
/*
阅读上面这段代码，我们发现 User 的 SetNameValueType 方法和 ValueSetName 方法，二者都是值传递，但是 SetNameValueType 方法会拷贝副本，ValueSetName 方法不会拷贝副本。
原因是我们给 ValueSetName 方法定义了一个 User 类型的返回值，从而避免了 ValueSetName 方法拷贝副本。
 */