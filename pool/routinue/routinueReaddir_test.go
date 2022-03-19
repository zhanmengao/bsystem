package routinue

import (
	"fmt"
	"io/ioutil"
	"os"
	"sync"
	"testing"
)

var (
	n    sync.WaitGroup
	done = make(chan struct{})
)

func cancelled() bool {
	select {
	case <-done:
		return true
	default:
		return false
	}
}

var tokenss = make(chan struct{}, 20) //token
func readDir(ch chan string, sizes chan int64) {
	defer n.Done()        //n--
	tokenss <- struct{}{} //get token
	path := <-ch
	<-tokenss
	fmt.Println("current path	", path)
	infos, err := ioutil.ReadDir(path)
	if err != nil {
		return
	}
	for _, info := range infos {
		if info.IsDir() {
			ch <- path + "/" + info.Name()
			n.Add(1)
			go readDir(ch, sizes)
		} else {
			sizes <- info.Size()
		}
	}
}

func Test_ReadDir(t *testing.T) {
	filelist := make(chan string, 20)
	sizes := make(chan int64, 20)
	path, _ := os.Getwd()
	for _, v := range os.Args[1:] {
		filelist <- v
	}
	filelist <- path
	n.Add(1) //n++
	go readDir(filelist, sizes)
	go func() {
		n.Wait() //等待n为0
		close(filelist)
		close(sizes)
	}()
	var total int64 = 0
	for v := range sizes {
		total += v
		fmt.Println(total)
	}

}
