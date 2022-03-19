package class

import (
	"fmt"
	"testing"
)

func TestBuffer(t *testing.T) {
	var b Buffer
	fmt.Fprintf(&b, "hello world")
}
