package refle

import (
	"fmt"
	"os"
	"reflect"
	"testing"
)

func TestFind(t *testing.T) {
	stdout := reflect.ValueOf(os.Stdout).Elem()
	fmt.Println(stdout.Type())

	fd:= stdout.FieldByName("pfd")
	fmt.Println(fd.Type())
	fmt.Println(fd.CanAddr(),fd.CanSet(),fd.CanInterface())
}
