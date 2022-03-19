package gostb

import (
	"fmt"
	"testing"
)
func TestTree(ts *testing.T) {
	var t RbTree
	t.Insert(9)
	t.Insert(2)
	t.Insert(6)
	t.Insert(33)
	t.CallAll(func(data RbData){
		fmt.Printf("data = %d\n",data)
	})
	fmt.Printf("%s \n",t.String())
}