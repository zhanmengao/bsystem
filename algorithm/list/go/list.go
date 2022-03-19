package gostb

import(
	"fmt"
)
type listCall func(n *Node)bool
type Node struct{
	val int
	pLast *Node
	pNext *Node
}
type List struct{
	head *Node				//表头
	tail *Node 				//队尾
	len int
}
func(v *List)PushBack(val int){
	if v.head == nil{
		v.head = new(Node)
		v.head.val = val
		v.tail = v.head
	}else{
		tail := new(Node)
		v.tail.pNext = tail
		tail.pLast = v.tail
		tail.val = val

		v.tail = tail
	}
}
func (v *List)Loop(call listCall){
	n := v.head
	for n!=nil{
		bRet:=call(n)
		if bRet{
			return
		}
		n = n.pNext
	}
}
func Find(node *Node,val int,index *int)(n* Node,err error){
	if node == nil{
		*index = -1
		return nil,fmt.Errorf("not found %d",val)
	}else if node.val == val{
		return node,nil
	} else{
		*index++
		return Find(node.pNext,val,index)
	}
}
func(v *List)Remove(val int){
	call := func(n *Node)bool{
		if n.val == val{
			if n.pLast != nil{
				n.pLast.pNext = n.pNext
			}
			if n.pNext !=nil{
				n.pNext.pLast = n.pLast
			}
			return true
		}
		return false
	}
	v.Loop(call)
}

func(v *List)Revert(){
	node:= v.head
	v.head = v.tail
	v.tail = node
	for node!=nil{
		next := node.pNext
		node.pNext = node.pLast
		node.pLast = node.pNext

		node = next
	}

}

func(v *List)Print(){
	call := func(n *Node)bool{
		fmt.Println(n.val)
		return false
	}
	v.Loop(call)
}