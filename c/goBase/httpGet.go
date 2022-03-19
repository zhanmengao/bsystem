package main

import(
    "fmt"		//include stdio.h
    //"io"
    "io/ioutil"
    "net/http"
    "os"
    "strings"
) 

func main(){
    f,err := os.Open("/home/wby/DOc/git.txt")
    if(err != nil){
        fmt.Fprintf(os.Stderr,"os.Open err %v \n",err)
    }
    for _,url := range os.Args[1:]{
        fmt.Println(url)
        if(!strings.Contains(url,"http")){
            url = "http://" + url
        }
        resp,err := http.Get(url)
        if( err!= nil){
            fmt.Fprintf(os.Stderr,"fetch %v \n",err)
            os.Exit(1)
        }
        //_,err := io.Copy(os.Stdout,resp.Body)    
        b,err := ioutil.ReadAll(resp.Body)
        var httpCode = resp.Status
        resp.Body.Close()
        if( err!= nil){
            fmt.Fprintf(os.Stderr,"fetch: reading %s : %v \n",url,err)
            os.Exit(1)
        }
        fmt.Printf("%s %s \n",httpCode,b)
    }
    f.Close()
}



