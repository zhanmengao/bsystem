package Function

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

func Test_MethodRecv(t *testing.T) {
	user1 := &User{}
	fmt.Printf("pointer:%p\n", user1) // pointer:0xc000096220
	fmt.Println(user1)                // &{}
	user1.SetNameValueType("lucy")
	fmt.Println(user1) // &{}
	user1.SetNamePointerType("lily")
	fmt.Println(user1) // &{lily}
}
