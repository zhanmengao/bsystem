package Interface

import (
	"fmt"
	"testing"
)
type MsgBase interface {
	GetId() int 				//接口上的方法突出了满足这个接口的具体类型之间的相似性，但隐藏了各个类型的布局和功能。强调接口方法
}
type MsgLogin struct{
	userName string
	password []byte
}
func(m MsgLogin)GetId()int{
	return int(MSG_LOGIN)
}

type MsgReg struct{
	userName string
	password []byte
}
func(m MsgReg)GetId()int{
	return int(MSG_REG)
}

type MSG_ID int
const(
	MSG_LOGIN MSG_ID = 1
	MSG_REG MSG_ID = 2
)
//接口值容纳各种具体类型的能力
func HandleMessage(m MsgBase){
	fmt.Printf("recv msg %d \n",m.GetId())
	switch m.GetId() {
	case int(MSG_LOGIN):{
		login,ok := m.(*MsgLogin)
		if ok{
			fmt.Println("Recv MSG_LOGIN",login.userName)
		}
	}
	case int(MSG_REG):{
		reg,ok:= m.(*MsgReg)
		if ok {
			fmt.Println("Recv MSG_LOGIN", reg.userName)
		}
	}
	}
}

func HandleSwitch(m MsgBase){
	switch m.(type) {
	case *MsgLogin:{
		login,ok := m.(*MsgLogin)
		if ok{
			fmt.Println("Recv MSG_LOGIN",login.userName)
		}

	}
	case *MsgReg:{
		login,ok:= m.(*MsgReg)
		if ok{
		fmt.Println("Recv MSG_LOGIN",login.userName)
		}
	}
	}
}

func Test_HandleMessage(t *testing.T){
	var login MsgLogin
	login.userName = "avoku"
	//HandleMessage(&login)
	HandleSwitch(&login)
}