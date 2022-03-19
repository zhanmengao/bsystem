package main

import "fmt"

//定义接口
type inter interface {
	//通信接口
	CSocketProtocol()
	//加密接口
	CEncDesProtocol()
}

//厂商1类
type CSckImp1 struct {
	data   string
	socket string
}

//厂商2类
type CSckImp2 struct {
	data   string
	socket string
	value int
}

func (cs1 *CSckImp1) CSocketProtocol() {
	fmt.Printf("厂商1的通信接口数据为：%s\n", cs1.socket)
}
func (cs1 *CSckImp1) CEncDesProtocol() {
	fmt.Printf("厂商1的加密接口数据为：%s\n", cs1.data)
}

func (cs2 *CSckImp2) CSocketProtocol() {
	fmt.Printf("厂商2的通信接口数据为：%s\n", cs2.socket)
}
func (cs2 *CSckImp2) CEncDesProtocol() {
	fmt.Printf("厂商2的加密接口数据为：%s 数值为：%d\n", cs2.data,cs2.value)
}

//多态实现
func framework(i inter) {
	i.CSocketProtocol()
	i.CEncDesProtocol()
}

func main() {
	cs1 := CSckImp1{"厂商1的加密数据", "厂商1的通信数据"}
	framework(&cs1)

	cs2 := CSckImp2{"厂商2的加密数据", "厂商2的通信数据",123}
	framework(&cs2)
}
