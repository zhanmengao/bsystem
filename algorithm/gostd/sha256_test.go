package arr

import (
	"crypto/sha256"
	"fmt"
	"testing"
)

func TestSha(t *testing.T) {
	c := sha256.Sum256([]byte("x"))
	PrintArrPoint(&c)
	fmt.Printf("Type : %T \n", c)
	PrintArr(c)
}
