package Function

import (
	"fmt"
	"golang.org/x/net/html"
	"net/http"
	"os"
	"testing"
)

func Test_Element(t *testing.T) {
	resp, err := http.Get("https://www.qq.com")
	if err != nil {
		fmt.Println(err)
		os.Exit(-1)
	}
	defer resp.Body.Close()

	doc, err := html.Parse(resp.Body)
	if err != nil {
		fmt.Println(err)
	}
	var call callNode = callElement //函数指针变量
	node := ElementByID(doc, "script", call)
	if node != nil {
		fmt.Printf("found node %v \n", *node)
	} else {
		fmt.Println("2222", "not found")
	}
}

type callNode func(n *html.Node, id string) bool //typedef一个函数指针类型
func ElementByID(n *html.Node, id string, call callNode) *html.Node {
	if call != nil {
		bRet := call(n, id)
		if bRet {
			return n
		}
	}

	for c := n.FirstChild; c != nil; c = c.NextSibling {
		ret := ElementByID(c, id, call)
		if ret != nil {
			return ret
		}
	}
	return nil
}

func callElement(n *html.Node, id string) bool {
	fmt.Println(n.Data)
	if n.Data == id {
		return true
	}
	return false
}
