package JobQueue

import (
	"fmt"
	"testing"
	"time"
)

// MaxWaitTime test
var MaxWaitTime = 10 * time.Millisecond

func BenchmarkChQueue(b *testing.B) {
	type dataItem struct {
		name string
		age  int
	}

	dataContainer := NewDataContainer(2)
	//add
	fmt.Printf("res=%v\n", dataContainer.Push(&dataItem{"zhangsan", 25}, MaxWaitTime))
	fmt.Printf("res=%v\n", dataContainer.Push(&dataItem{"lisi", 30}, MaxWaitTime))
	fmt.Printf("res=%v\n", dataContainer.Push(&dataItem{"wangwu", 28}, MaxWaitTime))

	//get
	var item interface{}
	item = dataContainer.Pop(MaxWaitTime)
	if item != nil {
		if tmp, ok := item.(*dataItem); ok { //interface转为具体类型
			fmt.Printf("item name:%v, age:%v\n", tmp.name, tmp.age)
		}
	}
}
