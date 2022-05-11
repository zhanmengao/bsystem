package freequeue

import (
	"sync/atomic"
	"unsafe"
)

type cList struct {
	head *node
}

func newList() *cList {
	return &cList{
		head: new(node),
	}
}

func (l *cList) insertHead(val interface{}) {
	nn := newNode(val)
	for {
		tmp := nn
		next := unsafe.Pointer(nn.next)
		if atomic.CompareAndSwapPointer(&next, unsafe.Pointer(tmp.next), unsafe.Pointer(l.head.next)) {
			l.head.next = tmp
		}
	}
}

func (l *cList) insertTail(val interface{}) {
	nn := newNode(val)
	cur := l.head
	for {
		pointCur := unsafe.Pointer(cur)
		if atomic.CompareAndSwapPointer(&pointCur, unsafe.Pointer(cur), unsafe.Pointer(cur)) {
			for ; cur.next != nil; {
				tmpLoop := cur
				atomic.CompareAndSwapPointer(&pointCur, unsafe.Pointer(tmpLoop), unsafe.Pointer(cur.next))
			}
			if atomic.CompareAndSwapPointer(&pointCur, unsafe.Pointer(cur), unsafe.Pointer(cur)) {
				cur.next = nn
			}
		}
	}
}


func(l *cList)remove(val interface{}){

}