package main

import (
	"fmt"
	"testing"
)

type student struct {
	Name string
	Age  int
}

func Test_Range(t *testing.T) {
	var stus []student

	stus = []student{
		{Name: "one", Age: 18},
		{Name: "two", Age: 19},
	}

	data := make(map[int]*student)

	for i, v := range stus {
		fmt.Printf("t addr %p v = %v \n",&v,v)
		data[i] = &v 					//应该改为：data[i] = &stus[i]
	}

	for i, v := range data {
		fmt.Printf("key=%d, value=%v \n", i, v)
	}
}

func Test_Range2(t *testing.T){
	var stus []student

	stus = []student{
		{Name: "one", Age: 18},
		{Name: "two", Age: 19},
	}

	data := make(map[int]*student)

	for i, v := range stus {
		fmt.Printf("t addr %p v = %v \n",&stus[i],v)
		data[i] = &stus[i]					//应该改为：data[i] = &stus[i]
	}

	for i, v := range data {
		fmt.Printf("key=%d, value=%v \n", i, v)
	}
}
