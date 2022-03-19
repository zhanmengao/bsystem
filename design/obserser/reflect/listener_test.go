package reflect

import (
	"container/list"
	"fmt"
	"reflect"
	"testing"
)

type EventCB func(uid string)
type eventMap map[int32]*list.List //value == EventCB

//函数指针和节点的映射，用于删除，因为go没有办法比较指针。。害
type elements map[reflect.Value]*list.Element
type elementsMap map[int32]elements

var (
	eMap   eventMap    = make(map[int32]*list.List, 0)
	eleMap elementsMap = make(map[int32]elements, 0) //value == map[函数指针]element
)

func RegisterEvent(eventID int32, cb EventCB) {
	if _, ok := eMap[eventID]; !ok {
		eMap[eventID] = list.New()
	}
	if cb != nil {
		ele := eMap[eventID].PushBack(cb)
		var v elements
		var ok bool
		if v, ok = eleMap[eventID]; !ok {
			v = make(elements, 0)
			eleMap[eventID] = v
		}
		v[reflect.ValueOf(cb)] = ele
	}
}

// UnRegisterEvent 函数类型没办法比较，先用反射实现
func UnRegisterEvent(eventID int32, cb EventCB) {
	var ll *list.List
	var ok bool
	var elems elements
	var ele *list.Element
	if ll, ok = eMap[eventID]; !ok {
		return
	}
	if elems, ok = eleMap[eventID]; !ok {
		return
	}
	pointer := reflect.ValueOf(cb)
	if ele, ok = elems[pointer]; !ok {
		return
	}
	ll.Remove(ele)
}

// NotifyEvent 事件发生
func NotifyEvent(eventID int32, uid string) {
	var ll *list.List
	var ok bool
	if ll, ok = eMap[eventID]; !ok {
		return
	}
	for item := ll.Front(); item != nil; item = item.Next() {
		cb, ok := item.Value.(EventCB)
		if ok && cb != nil {
			cb(uid)
		}
	}
}

func Hello(uid string) {
	fmt.Printf("hello %s \n", uid)
}

func Test_Listener(t *testing.T) {
	RegisterEvent(1, Hello)
	NotifyEvent(1, "22")
	UnRegisterEvent(1, Hello)
	NotifyEvent(1, "22")
}
