package main

import(
	"flag"
	"fmt"
	"strings"
	"testing"
)

var n = flag.Bool("n",false,"omit trailing newline")			//name = "n",default = false,desc = "ouit...."
var sep = flag.String("s","	","separator")						//name = "s",default = "	",desc = "separator"

func TestPoint(t *testing.T) {
	flag.Parse()												//update flag's dafault value
	fmt.Printf(strings.Join(flag.Args(),*sep))
	if !(*n){
		fmt.Println()
	}
}