package refle

import (
	"fmt"
	"reflect"
	"testing"
)

func TestElement(t *testing.T) {
	x := 2

	v := reflect.ValueOf(&x)
	fmt.Println(v.Kind(),"	",v.Type())

	e := v.Elem() 										//return value
	fmt.Println(e.Kind(),"	",e.Type())
	fmt.Printf("d(%v) = %v \n", reflect.TypeOf(e.Interface()), e)

	a := e.Addr()
	fmt.Println(a.Kind(),"	",a.Type())
}


func TestAddr(t *testing.T) {
	var data proto
	pv := reflect.ValueOf(&data)
	fmt.Println("pv =  ", pv.String())

	ele := pv.Elem()
	fmt.Println("ele = ", ele.String(), ele.Addr().Pointer())

	if ele.CanAddr() {
		addr := ele.Addr()
		fmt.Println("addr = ", addr.String(), addr.Pointer())
	}
}

func TestAddr2(t *testing.T) {
	var data proto

	v := reflect.ValueOf(data)
	fmt.Println(v.String())
	if v.CanAddr() {
		fmt.Println(v.Addr())
	}
}