package Interface

import (
	"fmt"
	"log"
	"net/http"
	"testing"
)

type dollars float32
type database map[string]dollars


func (d dollars) String() string {
	return fmt.Sprintf("$%.2f", d)
}
//ServeHTTP(ResponseWriter, *Request)
func(db database)ServeHTTP(w http.ResponseWriter,req *http.Request) {
	switch req.URL.Path{
	case "/list":db.ShowList(w,req)
	case "/price":db.Query(w,req)
	default:{
		w.WriteHeader(http.StatusNotFound)
		fmt.Fprintf(w,"no such page %s \n",req.URL)
	}
	}
}
func(db database)ShowList(w http.ResponseWriter,req *http.Request){
	for item,price := range db{
		fmt.Fprintf(w,"%s %s \n",item,price)
	}
}
func(db database)Query(w http.ResponseWriter,req *http.Request){
	item:= req.URL.Query().Get("item")
	price,ok:=db[item]
	if !ok{
		w.WriteHeader(http.StatusNotFound)
		fmt.Fprintf(w,"no such item: %q \n",item)
		return
	}
	fmt.Fprintf(w,"%s \n",price)
}



func Test_Http(t *testing.T) {
	db := database{"shoes": 50, "socks": 5}
	//http.ListenAndServe("127.0.0.1:8080",db)

	//erverMux
	mux:=http.NewServeMux()
	mux.Handle("/list",http.HandlerFunc(db.ShowList))
	mux.HandleFunc("/price",db.Query)
	log.Fatal(http.ListenAndServe("127.0.0.1:8080",mux))
}
