package main

import(
	"fmt"
	"os"
	"strings"
)

func main(){
	//for
	var s string
	for i := 0; i < len(os.Args); i++ {
		s += os.Args[i] + "\n"
	}
	fmt.Println(s)

	//while
	for i,arg := range os.Args[1:]{
		fmt.Printf("[%d] = %s \n",i,arg)
	}

	//use strings.Join gc
	fmt.Println(strings.Join(os.Args[1:],"	"))
}