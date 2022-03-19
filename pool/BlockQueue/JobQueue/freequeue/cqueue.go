package freequeue

import (
	"sync/atomic"
	"unsafe"
)

type node struct {
	value interface{}
	next  *node
}

func newNode(val interface{}) *node {
	return &node{
		value: val,
	}
}

type queue struct {
	front *node
	rear  *node
}

func newQueue() *queue {
	n := new(node)
	return &queue{
		front: n,
		rear:  n,
	}
}

func (q *queue) push(data interface{}) {
	newNode := &node{
		value: data,
	}
	for {
		point := unsafe.Pointer(q.rear.next)
		if atomic.CompareAndSwapPointer(&point, nil, unsafe.Pointer(newNode)) {
			q.rear = q.rear.next
		}
	}
}

func (q *queue) pop() interface{} {
	for {
		del := q.front.next
		if del == nil {
			return nil
		}
		point := unsafe.Pointer(q.front.next)
		if atomic.CompareAndSwapPointer(&point, unsafe.Pointer(del), unsafe.Pointer(del.next)) {
			// 队列中只剩一个结点
			if del.next == nil {
				q.front = q.rear
			}
			return del.value
		}
	}
}
