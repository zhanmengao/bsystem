package main

import (
	"fmt"
	"testing"
	"time"
)

func TestArray(t *testing.T) {
	uidArray := make([]int64, 0)
	tm := time.Now().Unix()
	for i := 0; i < 10000001; i++ {
		uidArray = append(uidArray, int64(i)+tm)
	}

	//分段遍历切片
	cursor := 0
	const each = 1000
	for cursor < len(uidArray) {
		next := cursor + each
		if next > len(uidArray) {
			next = len(uidArray)
		}
		data := uidArray[cursor:next]
		fmt.Printf("last = %d \n",data[len(data) - 1])
		cursor = next
	}
}
