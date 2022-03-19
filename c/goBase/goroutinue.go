package main

import(
    "fmt"		//include stdio.h
    "io"
    "io/ioutil"
    "net/http"
    "os"
    "strings"
    "time"
) 

func main(){
    var start = time.Now()
    ch:= make(chan string)          //channel
    for _,url := range os.Args[1:]{
        go fetch(url,ch)            //run routinue
    }
    for range os.Args[1:]{
        fmt.Println(<-ch)               //recv from channel
    }
    fmt.Printf("%.2fs elapsed\n",time.Since(start).Seconds())
}

func fetch(url string,ch chan<- string){
    var start = time.Now()
    if(!strings.Contains(url,"http")){
        url = "http://" + url
    }
    resp,err := http.Get(url)
    if( err!= nil){
        ch <- fmt.Sprintf("%s",err)          //send to channel
        return
    }
    nBytes,err := io.Copy(ioutil.Discard,resp.Body)   
    var httpCode = resp.Status
    resp.Body.Close()
    var secs = time.Since(start).Seconds();
    if( err!= nil){
        ch <- fmt.Sprintf("while reading %s : %v",url,err)          //send to channel
        return
    }
    ch <- fmt.Sprintf("%.2fs %7d %s : %s",secs,nBytes,url,httpCode)          //send to channel
}


