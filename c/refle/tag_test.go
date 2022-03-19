package refle

import (
	"encoding/json"
	"fmt"
	"reflect"
	"testing"
)

type User struct {
	UserId   int    `json:"user_id" bson:"user_id"`
	UserName string `json:"user_name" bson:"user_name"`
}

func TestPrintTag(t *testing.T) {
	u := &User{UserId: 1, UserName: "tony"}
	j, _ := json.Marshal(u)
	fmt.Println(string(j))
}



func TestTagGet(t *testing.T) {
	// 输出json格式
	u := &User{UserId: 1, UserName: "tony"}

	// 获取tag中的内容
	tp := reflect.TypeOf(u)
	field := tp.Elem().Field(0)
	fmt.Println(field.Tag.Get("json"))
	// 输出：user_id
	fmt.Println(field.Tag.Get("bson"))
	// 输出：user_id
}