package gotemplate

import (
	"io"
	"testing"
)

func TestQueue(t *testing.T) {
	var q1 TQueue[int] // 可存放int类型数据的队列
	q1.Put(1)
	q1.Put(2)
	q1.Put(3)
	q1.Pop() // 1
	q1.Pop() // 2
	q1.Pop() // 3

	var q2 TQueue[string] // 可存放string类型数据的队列
	q2.Put("A")
	q2.Put("B")
	q2.Put("C")
	q2.Pop() // "A"
	q2.Pop() // "B"
	q2.Pop() // "C"

	_ = TQueue[struct{ Name string }]
	_ = TQueue[[]int]     // 可存放[]int切片的队列
	_ = TQueue[chan int]  // 可存放int通道的队列
	_ = TQueue[io.Reader] // 可存放接口的队列
}
