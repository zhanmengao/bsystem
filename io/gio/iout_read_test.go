package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"strings"
	"testing"
)

func TestIORead(t *testing.T) {
	content, err := ioutil.ReadAll(strings.NewReader("hello world !"))

	if err != nil {
		log.Fatal(err)
	}

	fmt.Print(string(content))
}
