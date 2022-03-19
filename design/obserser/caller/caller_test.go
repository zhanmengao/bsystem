package caller

import (
	"container/list"
	"fmt"
	"testing"
)
// EventCaller 函数对象
type EventCaller interface {
	Call(uid string)
}

type EventCB func(uid string)

func (f EventCB) Call(uid string) {
	f(uid)
}
type eventMap map[int32]*list.List

var (
	eMap eventMap = make(map[int32]*list.List)
)

func RegisterEvent(eventID int32, caller EventCaller) {
	if _, ok := eMap[eventID]; !ok {
		eMap[eventID] = list.New()
	}
	if caller != nil {
		eMap[eventID].PushBack(caller)
	}
}

// UnRegisterEvent 调用频率应该不会太高，按需优化
func UnRegisterEvent(eventID int32, cb EventCaller) {
	var ll *list.List
	var ok bool
	if ll, ok = eMap[eventID]; !ok {
		return
	}
	for item := ll.Front(); item != nil; item = item.Next() {
		if item.Value.(EventCaller) == cb {
			ll.Remove(item)
			return
		}
	}
}

// NotifyEvent 事件发生
func NotifyEvent(eventID int32, uid string) {
	var ll *list.List
	var ok bool
	if ll, ok = eMap[eventID]; !ok {
		return
	}
	for item := ll.Front(); item != nil; item = item.Next() {
		caller, ok := item.Value.(EventCaller)
		if ok && caller != nil {
			caller.Call(uid)
		}
	}
}
func Hello(uid string){
	fmt.Printf("hello %s \n" ,uid)
}
func Test_Caller(t *testing.T) {
	RegisterEvent(1,EventCB(Hello))
	NotifyEvent(1,"22")
	UnRegisterEvent(1,EventCB(Hello))
	NotifyEvent(1,"22")
}
