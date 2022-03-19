package class

import (
	"fmt"
	"testing"
)

func TestIntset(t *testing.T) {
	var s IntSet
	s.AddAll(1, 2, 3, 4, 9, 5)
	fmt.Printf("%s \n", s.String())
}
