package arr

import (
	"container/list"
	"fmt"
	"reflect"
	"testing"
)

type elements map[reflect.Value]*list.Element

type CallBack func()
var (
	eleMap = make(map[int32]elements, 0) //函数指针和节点的映射
)

func Hello() {
	fmt.Printf("hello \n")
}

func Test_MapPointer(t *testing.T) {
	var fun CallBack = Hello
	ll := list.New()
	ele := ll.PushBack(fun)
	eleMap[0] = make(map[reflect.Value]*list.Element, 0)
	eleMap[0][reflect.ValueOf(fun)] = ele

	//取
	var fun2 CallBack = Hello
	ele = eleMap[0][reflect.ValueOf(fun2)]
	fmt.Println()
	cb := ele.Value.(CallBack)
	cb()
}
