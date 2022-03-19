package Function

import (
	"fmt" //include stdio.h
	"testing"

	//"io"
	"io/ioutil"
	"net/http"
	"os"
	"strings"
)

func httpGet(url string) error {
	if !strings.Contains(url, "http") {
		url = "http://" + url
	}
	resp, err := http.Get(url)
	if err != nil {
		fmt.Fprintf(os.Stderr, "fetch %v \n", err)
		return err
	}
	defer resp.Body.Close()
	var b []byte
	b, err = ioutil.ReadAll(resp.Body)
	var httpCode = resp.Status
	if err != nil {
		fmt.Fprintf(os.Stderr, "fetch: reading %s : %v \n", url, err)
		return err
	}
	fmt.Printf("%s %s \n", httpCode, b)
	return nil
}
func Test_defer(t *testing.T) {
	for _, url := range os.Args[1:] {
		fmt.Println(url)
		httpGet(url)
	}
}

func Test_deferFifo(t *testing.T) {
	defer func() {
		fmt.Println("1")
	}()

	defer func() {
		fmt.Println("2")
	}()

	defer func() {
		fmt.Println("3")
	}()
}

func Test_deferFunction(t *testing.T){
	a := 1
	defer print(function(a))
	a = 2
}
func function(num int) int{
	fmt.Println("function call")
	return num
}
func print(num int){
	fmt.Println(num)
}


