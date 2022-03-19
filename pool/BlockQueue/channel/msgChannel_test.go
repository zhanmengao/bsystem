package main

import (
	"fmt"
	"os"
	"sync"
	"testing"
	"time"
)

func makeThumbnail(filenames <-chan string) (total int64) {
	sizes := make(chan int64)
	var wg sync.WaitGroup
	for f := range filenames {
		wg.Add(1)

		go func(f string) {
			defer wg.Done() //wg --
			time.Sleep(1)
			fmt.Printf("%s work end \n", f)
			info, _ := os.Stat(f)
			sizes <- info.Size() //write file size
		}(f)
	}

	//wait wg
	go func() {
		wg.Wait()
		close(sizes)
	}()

	//read sizes
	for sz := range sizes {
		total += sz
	}
	return total
}

func Test_MsgChannel(t *testing.T) {
	files := make(chan string)
	for _, f := range os.Args {
		files <- f
	}
	makeThumbnail(files)
}
