package main

import (
	"fmt"
	"log"
	"os"
	"gopl.io/ch5/links"
)

var tokens = make(chan struct{},20)				//token


func crawl(url string) []string {
	fmt.Println(url)
	tokens <-struct{}{}							//get token
	list, err := links.Extract(url)				//调用爬虫
	<-tokens									//release token
	if err != nil {
		log.Print(err)
	}
	return list
}


func main() {
	worklist := make(chan []string)
	worklist <-os.Args[1:]

	//并法爬取web
	seen:= make(map[string]bool)
	for list := range worklist{
		for _,link := range list{
			if !seen[link]{
				seen[link] = true
				go func(link string){				//为每一个url建立routinue		//
					worklist <- crawl(link)
				}(link)
			}
		}
	}
}

