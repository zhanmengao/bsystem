package gotemplate

import (
	"log"
	"testing"
)

func Foo[T any, PT interface {
	*T
	M()
}]() {
	p := PT(new(T))
	p.M() // calling method on non-nil pointer
}

type TT struct {
}

func (t *TT) M() {
	log.Println("M")
}
func TestT(t *testing.T) {
	Foo[TT]()
}
