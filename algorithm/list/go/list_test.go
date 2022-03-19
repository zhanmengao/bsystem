package gostb

import (
	"fmt"
	"testing"
)

func TestName(t *testing.T) {
	var list List
	for i:=0;i<100;i++{
		list.PushBack(i)
	}
	list.Remove(98)
	//list.Print()
	list.Revert()
	list.Print()
	var index = 0
	_,err:=Find(list.head,3,&index)
	fmt.Printf("index = %d err = %v",index,err)
}
