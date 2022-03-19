package gio

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"testing"
)

func TestReadBlock(t *testing.T) {
	pf, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer pf.Close()
	reader := bufio.NewReader(pf)

	chunks := make([]byte, 0)
	buf := make([]byte, 1024) 						//一次读取多少个字节
	for {
		n, err := reader.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if 0 == n {
			break
		}
		chunks = append(chunks, buf[:n]...)
	}
	fmt.Println(string(chunks))
}

func TestReadBlock2(t *testing.T) {
	pf, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer pf.Close()

	chunks := make([]byte, 0)
	buf := make([]byte, 1024)
	for {
		n, err := pf.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if 0 == n {
			break
		}
		chunks = append(chunks, buf[:n]...)
	}
	fmt.Println(string(chunks))
}