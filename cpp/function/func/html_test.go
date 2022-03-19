package Function

import (
	"fmt"
	"golang.org/x/net/html"
	"net/http"
	"testing"
)

func visit(links []string,n *html.Node)[]string{
	if n.Type == html.ElementNode && n.Data == "a"{
		for _,a := range n.Attr{
			if a.Key == "href"{
				links = append(links,a.Val)
			}
		}
	}
	for c:=n.FirstChild;c!=nil;c = c.NextSibling{
		links = visit(links,c)
	}
	return links
}
func findLinks(url string)([]string,error){
	resp,err :=http.Get(url)
	if err!=nil{
		return nil,fmt.Errorf("http get %s :%v",url,err)
	}
	defer resp.Body.Close()
	if resp.StatusCode != http.StatusOK{
		return nil,fmt.Errorf("getting %s :%s",url,resp.Status)
	}
	var doc *html.Node = nil
	doc,err = html.Parse(resp.Body)
	if err!=nil{
		return nil,fmt.Errorf("parsing %s as HTML : %v",url,err)
	}
	return visit(nil,doc),nil
}

func Test_Html(t *testing.T) {
	ss,_ := findLinks("https://www.qq.com")
	for s := range ss{
		fmt.Println(s)
	}
}