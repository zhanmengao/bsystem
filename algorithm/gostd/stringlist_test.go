package arr

import (
	"fmt"
	"testing"
)

func TestStringList(t *testing.T) {
	medals := []string{"gold", "silver", "bronze"}
	for i := len(medals) - 1; i >= 0; i-- {
		fmt.Println(medals[i])
	}
}
