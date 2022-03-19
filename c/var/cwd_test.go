package main

import (
	"log"
	"os"
	"testing"
)

var cwd string  //static

func init(){
	var err error
	//	cwd,err := os.Getwd();     //error use local cwd
	cwd,err = os.Getwd();
	if err!=nil{
		log.Fatalf("os.Getwd failed %v ",err)
	}
	log.Printf("Working directory = %s",cwd)
}
func TestCwd(t *testing.T) {
	log.Printf("main : Working directory = %s",cwd)
}