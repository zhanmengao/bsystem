package routinue

import (
	"fmt"
	"gopl.io/ch5/links"
	"log"
	"os"
	"testing"
)

var tokens2 = make(chan struct{},20)				//token


func craw2(url string) []string {
	fmt.Println(url)
	tokens2 <-struct{}{}							//get token
	list, err := links.Extract(url)				//调用爬虫
	<-tokens2									//release token
	if err != nil {
		log.Print(err)
	}
	return list
}


func Test_RoutinuePool(t *testing.T) {
	worklist := make(chan []string)					//可能有重复的Url列表
	unseenLinks := make(chan string)				//去除重复的Url列表

	go func(){
		worklist<-os.Args[1:]
	}()
	for i:=0;i<20;i++{			//限制routinue数量，防止多度并发
		go func(){
			for link:=range unseenLinks{
				fountLinks := craw2(link)
				go func(){
					worklist <-fountLinks
				}()
			}
		}()
	}

	seen:=make(map[string]bool)
	for list:=range worklist{
		for _,link :=range list{
			if !seen[link]{
				seen[link] = true
				unseenLinks <- link
			}
		}
	}
}

