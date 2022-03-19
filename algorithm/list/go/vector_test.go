package gostb

import "testing"

func TestVector(t *testing.T) {
	var v vector
	for i := 0; i < 100; i++ {
		v.PushBack(i)
	}
	v.Print()
}
