package main

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"reflect"

	"bou.ke/monkey"
)

func main() {
	var d *net.Dialer
	monkey.PatchInstanceMethod(reflect.TypeOf(d), "Dial", func(_ *net.Dialer, _, _ string) (net.Conn, error) {
		fmt.Println("On hook function")
		return nil, fmt.Errorf("no dialing allowed")
	})
	data, err := http.Get("http://google.com")
	if err != nil {
		log.Println(err)			// Get http://google.com: no dialing allowed
	} else {
		log.Println(data.Status)
	}
}
