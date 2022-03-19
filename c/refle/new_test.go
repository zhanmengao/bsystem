package refle

import (
	"fmt"
	"os"
	"reflect"
	"testing"
)

func TestNew(t *testing.T) {
	tp := reflect.TypeOf(os.Stdout).Elem()
	nt := reflect.New(tp)
	fmt.Println(nt)
	iFile := nt.Interface()
	fmt.Println(display("", nt))
	fmt.Println(iFile)
}
