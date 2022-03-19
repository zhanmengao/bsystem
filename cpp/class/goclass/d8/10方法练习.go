package main

import (
	"fmt"
)

/*
记者：我是记者  我的爱好是偷拍 我的年龄是34 我是一个男狗仔
程序员：我叫孙权 我的年龄是23 我是男生 我的工作年限是 3年
 */

//父类
type Human struct {
	name string
	age  int
	sex  string
}

//记者子类
type Reper struct {
	Human

	hobby string
}

//程序员子类
type Dever struct {
	Human

	worktime int
}

//父类方法
func (h *Human) SayHi() {
	fmt.Printf("大家好，我叫%s，我今年%d岁，我是%s生", h.name, h.age, h.sex)
}

//记者的方法
func (r *Reper) SayHello() {
	//继承父类方法
	r.SayHi()
	fmt.Printf("，我的爱好是：%s\n", r.hobby)

}

//程序员的方法
func (d *Dever) SayHello() {
	d.SayHi()
	fmt.Printf("，我的工作年限是：%d年\n", d.worktime)
}
func main() {

	r:=Reper{Human{"卓伟",40,"男"},"偷拍"}
	r.SayHello()

	d:=Dever{Human{"图灵",32,"男"},10}
	d.SayHello()

}
