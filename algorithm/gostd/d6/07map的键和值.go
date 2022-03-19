package main

import "fmt"

func main0701() {

	//arr:=[2]int{1,2}
	//map中的key类型必须支持== !=  一遍建议写基本类型
	//map中的数据是无序的
	m := make(map[string][3]int)

	m["小明"] = [3]int{100, 99, 100}
	m["小罗"] = [3]int{3, 4, 5}
	m["乾隆"] = [3]int{101, 101, 101}

	//fmt.Println(m)

	for _, v := range m {
		//fmt.Println(k,v)

		//fmt.Printf("%T\n",v)
		//fmt.Printf("姓名：%s  语文：%d 数学：%d 英语：%d\n", k, v[0], v[1], v[2])

		for i := 0; i < len(v); i++ {
			fmt.Println(v[i])
		}
	}
}
func main0702() {
	m := make(map[int]string)

	m[101] = "刘备"
	m[102] = "关羽"
	m[103] = "李逵"

	//在map中只能通过key找到值
	//fmt.Println(m["李逵"])//err
	//在map中如果没有提供key找到具体的值 打印value类型的默认值
	//fmt.Println(m[105])

	//在毛重可以判断key和值是否存在
	value, ok := m[101]

	fmt.Printf("%T\n",ok)

	if ok {
		fmt.Println(value)
	} else {
		fmt.Println("未找到数据")
	}
}

func main() {
	m := make(map[int]string)

	m[101] = "刘备"
	m[102] = "关羽"
	m[103] = "李逵"

	//删除map中的一个元素 根据key进行删除
	delete(m,103)
	delete(m,102)
	delete(m,101)
	//delete在进行数据删除时 如果key不存在 不会报错
	delete(m,101)

	fmt.Println(m)

}