package class

import (
	"fmt"
	"testing"
)
type student struct{
	Name string
	Age  int
}

func (p *student) love(){
	fmt.Println("love")

}

func (p *student) like(){
	fmt.Println("like first")
	p.love()
}

type boy struct {
	student
}

func (b * boy) love(){
	fmt.Println("hate")
}
func TestClass(t *testing.T) {
	b := boy{}

	b.like()			//不会重写

}

func TestClass2(t *testing.T){
	b := boy{}

	b.love()
}
