package refle

import (
	"fmt"
	"io"
	"os"
	"reflect"
	"testing"
)

func TestTypeOf(ttt *testing.T){
	t := reflect.TypeOf(3)
	fmt.Println(t)
	fmt.Println(t.String())

	var w io.Writer = os.Stdout
	fmt.Println(reflect.TypeOf(w))
}


func TestValueOf(ttt *testing.T){
	v := reflect.ValueOf(3)		//return value
	fmt.Println(v)
	fmt.Println(v.String())
	fmt.Println(v.Type())

	i := v.Interface()
	val := i.(int)
	fmt.Println(val)
}
