package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

func main() {
	counts := make(map[string]int)
	for _, filePath := range os.Args[1:] {
		data, err := ioutil.ReadFile(filePath)
		if err != nil {
			fmt.Fprintf(os.Stderr, "dup3: %v \n", err)
			continue
		}
		for _, str := range strings.Split(string(data), "\n") {
			counts[str]++
		}
	}
	for line, n := range counts {
		if n > 1 {
			fmt.Printf("%d \t %s \n", n, line)
		}
	}
}
