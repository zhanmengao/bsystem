package main

import (
	"fmt"
	"syscall"
)

func main() {
	env := syscall.Environ()
	for _, v := range env {
		fmt.Println(v)
	}
}
