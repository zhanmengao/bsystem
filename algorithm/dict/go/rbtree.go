package gostb

import "fmt"
import "strconv"
type RbData int
const (
	BLACK = false
	RED = true
)
type rbColor bool
type rbNode struct{
	data RbData
	left *rbNode
	right *rbNode
	col rbColor
}

type RbTree struct{
	root *rbNode
	sz int
}

func(t *RbTree)Insert(data RbData){
	t.root = insert(t.root,data)
	t.root.col = BLACK
}
func(t *RbTree) CallAll(call Call) {
	callAll(t.root,call)
}
func(t *RbTree)String()string{
	fmt.Println("RbTree's String")
	var s string = "{"
	t.CallAll(func(data RbData){
		s += strconv.Itoa(int(data))
		s +=" "
	})
	s+="}"
	return s
}


func insert(root *rbNode,data RbData)(*rbNode){
	if root == nil{
		return &rbNode{data: data,col:RED,right: nil,left: nil}
	}
	if root.data >= data{
		root.left = insert(root.left,data)
	}else if root.data<data {
		root.right = insert(root.right, data)
	}

	if isRed(root.right) && !isRed(root.left){
		root = rotateLeft(root)
	}
	if isRed(root.left) && isRed(root.left.left){
		root = rotateRight(root)
	}
	if isRed(root.left) && isRed(root.right){
		filpColor(root)
	}
	return root
}

func rotateLeft(h *rbNode)(*rbNode){
	x := h.right
	h.right = x.left
	x.left = h

	x.col = h.col
	h.col = RED
	return x
}
func rotateRight(h *rbNode)(*rbNode){
	x:=h.left
	h.left = x.right
	x.right = h

	x.col = h.col
	h.col = RED
	return x
}
func filpColor(h *rbNode){
	h.col = RED
	h.left.col = BLACK
	h.right.col = BLACK
}
func isRed(h *rbNode)bool{
	if h == nil{
		return false
	}else{
		return h.col == RED
	}
}
type Call func(RbData)
func callAll(h* rbNode,call Call){
	if h!=nil{
		call(h.data)
	}
	if h.left!=nil{
		callAll(h.left,call)
	}
	if h.right!=nil{
		callAll(h.right,call)
	}
}