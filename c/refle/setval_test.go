package refle

import (
	"fmt"
	"reflect"
	"testing"
	"unsafe"
)

func TestSetVal(t *testing.T) {
	x := 1
	rx := reflect.ValueOf(&x).Elem()
	fmt.Println(rx)
	rx.SetInt(2)
	fmt.Println(rx)
	rx.Set(reflect.ValueOf(3))
	fmt.Println(rx)

	var y interface{}
	ry := reflect.ValueOf(&y).Elem()
	//ry.SetInt(2)		dump
	ry.Set(reflect.ValueOf(3))
	fmt.Println(ry)
	ry.Set(reflect.ValueOf("string ?? yes"))
	fmt.Println(ry)
}

func TestGetVal(t *testing.T) {
	x := 2
	v := reflect.ValueOf(&x)
	d := v.Elem()
	//get value
	if d.CanAddr() {
		px := d.Addr().Interface().(*int)
		*px = 3
		fmt.Println(*px)
	}
}

func fillPointerYison(ptr interface{}) {
	str := "ok"
	v := reflect.ValueOf(ptr)
	vAddr := v.Elem().Field(0).Addr()
	fmt.Println(vAddr.String())
	addr := unsafe.Pointer(vAddr.Pointer())
	ppstr := (**string)(addr)
	*ppstr = &str
}

func fillPointer2(ptr interface{}) {
	str := "lan"
	v := reflect.ValueOf(ptr)
	v.Elem().Field(0).SetPointer(unsafe.Pointer(&str))
}

type PProto struct {
	Str unsafe.Pointer
}

func TestFillRsp(t *testing.T) {
	var pb proto
	fillPointerYison(&pb)
	fmt.Println(*pb.str)

	var Pb PProto
	fillPointer2(&Pb)
	pstr := (*string)(Pb.Str)
	fmt.Println(*pstr)
}
