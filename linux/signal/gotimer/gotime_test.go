package main

import (
	"fmt"
	"testing"
	"time"
)

var s int64 = 1624474800
var ms int64 = 1624474800000
func TestName(t *testing.T) {
	fmt.Printf("tm = %v \n",time.Unix(s,0))
	fmt.Printf("tm = %v \n",time.Unix(0,ms))
}
