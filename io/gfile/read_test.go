package gio

import (
	"bufio"
	"fmt"
	"io"
	"io/ioutil"
	"os"
	"testing"
	"time"
)

var (
	bigfile string
)

func init() {
	bigfile, _ = os.Getwd()
	bigfile += "/../../Linux/操作系统基础.pdf"
}
func read0(path string) string {
	file, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer file.Close()
	content, err := ioutil.ReadAll(file)
	return string(content)
}

func read1(path string) string {
	content, err := ioutil.ReadFile(path)
	if err != nil {
		panic(err)
	}
	return string(content)
}

func read2(path string) string {
	fi, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer fi.Close()
	r := bufio.NewReader(fi)

	chunks := make([]byte, 0)
	buf := make([]byte, 1024) //一次读取多少个字节
	for {
		n, err := r.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if 0 == n {
			break
		}
		chunks = append(chunks, buf[:n]...)
	}
	return string(chunks)
}

func read3(path string) string {
	fi, err := os.Open(path)
	if err != nil {
		panic(err)
	}
	defer fi.Close()

	chunks := make([]byte, 0)
	buf := make([]byte, 1024)
	for {
		n, err := fi.Read(buf)
		if err != nil && err != io.EOF {
			panic(err)
		}
		if 0 == n {
			break
		}
		chunks = append(chunks, buf[:n]...)
	}
	return string(chunks)
}

func BenchmarkRead(b *testing.B) {

	println(b.N)
	start := time.Now()
	for i := 0; i < b.N; i++ {
		read0(bigfile)
	}

	t0 := time.Now()
	fmt.Printf("Cost time %v\n", t0.Sub(start))

	for i := 0; i < b.N; i++ {
		read1(bigfile)
	}
	t1 := time.Now()
	fmt.Printf("Cost time %v\n", t1.Sub(t0))

	for i := 0; i < b.N; i++ {
		read2(bigfile)
	}
	t2 := time.Now()
	fmt.Printf("Cost time %v\n", t2.Sub(t1))

	for i := 0; i < b.N; i++ {
		read3(bigfile)
	}
	t3 := time.Now()
	fmt.Printf("Cost time %v\n", t3.Sub(t2))

}
