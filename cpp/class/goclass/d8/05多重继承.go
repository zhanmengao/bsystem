package main

import "fmt"

type human struct {
	id   int
	name string
}
type person4 struct {
	human

	age  int
	sex  string
	addr string
}

type student4 struct {
	person4

	class int
	score int
}

func main0501() {
	//var stu student4
	//
	//stu.name = "魏璎珞"
	//stu.sex = "女"
	//stu.addr = "皇宫"
	//stu.class = 3004
	//stu.score = 100
	//stu.age = 18
	//stu.id = 1001

	stu:=student4{person4{human{1001,"魏璎珞"},18,"女","皇宫"},3004,100}

	fmt.Println(stu)

}


/*

type 技能 struct{
	名称
	范围
	CD
	消耗
	伤害
	buff

}
type 人物信息 struct{

	姓名
	等级
	经验
	hp
	mp
	金钱

	skills []技能
}

 */

/*


type 消费记录 struct{

	时间
	金额
	地点
	流水号
	备注

}
type 信用卡 struct{

	卡号
	额度
	信誉度
	密码
	持卡人
	日期

	记录 []消费记录
}

 */