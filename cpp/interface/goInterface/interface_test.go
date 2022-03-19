package Interface

import (
	"fmt"
	"io"
	"testing"
)


type LimitReadr struct{
	r io.Reader
	nMax int64
}

func LimitRead(r io.Reader,n int64)io.Reader{
	return &LimitReadr{r,n}
}
func(l* LimitReadr)Read(p []byte)(n int,err error){
	if l.nMax<=0{
		return 0,io.EOF
	}
	if int64(len(p)) > l.nMax {
		p = p[0:l.nMax]
	}
	n, err = l.r.Read(p)
	l.nMax -= int64(n)
	return
}

type people interface {
	speak()
}

type student struct{
	name string
	age int
}
func (stu *student) speak(){
	fmt.Println("I am a student, I am ", stu.age)
}


func Test_Interface(t *testing.T) {
	var p people
	//p = student{name:"RyuGou", age:12}
	p = &student{name:"RyuGou", age:12} 			//应该改为 p = &student{name:"RyuGou", age:12}
	p.speak()
}