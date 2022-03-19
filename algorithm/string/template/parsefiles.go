package main

import (
	"fmt"
	"html/template"
	"log"
	"net/http"
)

func tmpl(w http.ResponseWriter, r *http.Request) {
	fmt.Printf("url %s \n",r.URL)
	t1, err := template.ParseFiles("test.html")
	if err != nil {
		panic(err)
	}
	t1.Execute(w, "hello world")
}

func main() {
	server := http.Server{
		Addr: "127.0.0.1:8088",
	}
	http.HandleFunc("/tmpl", tmpl)
	log.Fatalln(server.ListenAndServe())
}