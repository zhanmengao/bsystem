package main

import "fmt"

type student struct {
	name  string
	sex   string
	age   int
	score int
	addr  string
}

func main0101() {
	//将结构体作为map中的value
	m := make(map[int]student)

	//map中的数据不建议排序操作
	m[101] = student{"擎天柱", "男", 30, 100, "赛博坦星球"}
	m[102] = student{"大黄蜂", "男", 10, 59, "赛博坦星球"}

	//fmt.Println(m)

	//fmt.Println(m[101])

	delete(m, 101)
	fmt.Println(m)
}

func main0102() {
	//value类型是一个切片
	m := make(map[int][]student)

	m[101] = []student{student{"擎天柱", "男", 30, 100, "赛博坦星球"},
		student{"大黄蜂", "男", 10, 59, "赛博坦星球"}}

	m[101] = append(m[101], student{"伯爵", "男", 30, 88, "赛博坦星球"})

	m[102] = []student{student{"水兵月", "女", 18, 100, "月亮"}}
	m[102] = append(m[102], student{"月野兔", "女", 20, 99, "地球"})
	fmt.Println(m[102])
}
