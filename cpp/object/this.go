package main

import (
	"fmt"
)
import "C"
import _"unsafe"

type test struct {
	name string
}

func (t *test) getName() {
	fmt.Println("hello world")
}
func main() {
	var t *test
	t = nil
	t.getName()

	(*test).getName(nil)
}
