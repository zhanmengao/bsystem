package Function

import (
	"fmt"
	"reflect"
	"testing"
)

type helloType func()

func Hello() {
	fmt.Printf("hello go")
}
func Hello2() {
	fmt.Printf("hello go")
}
func TestFunc(t *testing.T) {
	var hello helloType = Hello
	var hello2 helloType = Hello
	fmt.Println(reflect.ValueOf(hello).Pointer() == reflect.ValueOf(hello2).Pointer())
}

func SomeFun()    {}
func AnotherFun() {}
func Test2(t *testing.T) {
	sf1 := reflect.ValueOf(SomeFun)
	sf2 := reflect.ValueOf(SomeFun)
	fmt.Println(sf1.Pointer() == sf2.Pointer())
	af1 := reflect.ValueOf(AnotherFun)
	fmt.Println(sf1.Pointer() == af1.Pointer())
}
