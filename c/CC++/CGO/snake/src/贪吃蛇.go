package main

import (
	"Clib"
	"fmt"
	"math/rand"
	"os"
	"time"
)

//全局常量 界面大小
const WIDE int = 20
const HIGH int = 20

//定义全局变量存储食物
var food Food

//初始化父类 坐标
type Position struct {
	X int
	Y int
}

//初始化蛇子类
type Snake struct {
	size int                   //长度
	dir  byte                  //方向
	pos  [WIDE * HIGH]Position //定义数组存储每一节蛇的坐标
}

//初始化食物子类
type Food struct {
	Position
}

//初始化蛇信息
func (s *Snake) SnakeInit() {
	//蛇的长度
	s.size = 2
	//蛇头位置
	s.pos[0].X = WIDE / 2
	s.pos[0].Y = HIGH / 2

	s.pos[1].X = WIDE/2 - 1
	s.pos[1].Y = HIGH / 2
	//蛇的方向
	//用 U上 D下 L左 R右
	s.dir = 'R'

	//绘制蛇的UI
	for i := 0; i < s.size; i++ {
		var ch byte
		//区分蛇头和身体
		if i == 0 {
			ch = '@'
		} else {
			ch = '*'
		}
		ShowUI(s.pos[i].X, s.pos[i].Y, ch)
	}
	//go 添加一个独立的函数  非阻塞执行
	//接收键盘中的输入
	go func() {
		for {
			switch Clib.Direction() {
			//根据键盘输入设置方向
			//上
			case 72, 87, 119:
				s.dir = 'U'
				//下
			case 80, 83, 115:
				s.dir = 'D'
				//左
			case 65, 75, 97:
				s.dir = 'L'
				//右
			case 68, 77, 100:
				s.dir = 'R'
			}
		}
	}()

}

func (s *Snake) PlayGame() {
	//更新蛇坐标的偏移值
	var dx, dy int = 0, 0
	//游戏流程控制
	for {
		//延迟执行 延迟333秒
		time.Sleep(time.Second / 6)
		//更新蛇的位置
		switch s.dir {
		case 'U':
			dx = 0
			dy = -1
		case 'D':
			dx = 0
			dy = 1
		case 'L':
			dx = -1
			dy = 0
		case 'R':
			dx = 1
			dy = 0
		}

		//蛇头和墙碰撞
		if s.pos[0].X < 0+1 || s.pos[0].X >= WIDE+1 || s.pos[0].Y < 0 || s.pos[0].Y >= HIGH {
			return
		}
		//蛇头和身体碰撞
		for i := 1; i < s.size; i++ {
			if s.pos[0].X == s.pos[i].X && s.pos[0].Y == s.pos[i].Y {
				return
			}
		}
		//记录末尾坐标
		lx := s.pos[s.size-1].X
		ly := s.pos[s.size-1].Y

		//蛇头和食物碰撞
		if s.pos[0].X == food.X && s.pos[0].Y == food.Y {
			//身体增长
			s.size++
			//随机新食物
			RandomFood()
			//分数
		}


		//更新蛇的坐标   蛇身体坐标
		for i := s.size - 1; i > 0; i-- {
			s.pos[i].X = s.pos[i-1].X
			s.pos[i].Y = s.pos[i-1].Y
		}
		//蛇头坐标
		s.pos[0].X += dx
		s.pos[0].Y += dy

		//绘制蛇的UI
		for i := 0; i < s.size; i++ {
			var ch byte
			//区分蛇头和身体
			if i == 0 {
				ch = '@'
			} else {
				ch = '*'
			}
			ShowUI(s.pos[i].X, s.pos[i].Y, ch)
		}
		//将末尾置为空
		ShowUI(lx, ly, ' ')
	}

}

func RandomFood() {
	//随机食物
	//重合之后重新随机
	food.X = rand.Intn(WIDE) + 1 //0-19
	food.Y = rand.Intn(HIGH) + 1
	//显示食物位置
	ShowUI(food.X, food.Y, '#')
}

//初始化地图
func MapInit() {
	// 输出初始画面
	fmt.Fprintln(os.Stderr,
		`
  #-----------------------------------------#
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  |                                         |
  #-----------------------------------------#
`)
}

//显示界面信息
func ShowUI(X int, Y int, ch byte) {
	//调用C语言代码 设置控制台光标
	//根据地图坐标有偏移
	Clib.GotoPostion(X*2+2, Y+2)
	//将字符绘制在界面中
	fmt.Fprintf(os.Stderr, "%c", ch)
}
func main() {
	//设置随机数种子  用作于混淆
	rand.Seed(time.Now().UnixNano())
	//隐藏控制台光标
	Clib.HideCursor()

	//初始化地图
	MapInit()
	//随机食物
	RandomFood()

	//创建蛇对象
	var s Snake
	//初始化蛇信息
	s.SnakeInit()

	s.PlayGame()

}
