package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"testing"
	"time"
)

func TestTmpFile(t *testing.T) {
	wd, _ := os.Getwd()
	f, _ := ioutil.TempFile(wd, "tmp")
	defer f.Close()
	fmt.Printf("tmpfile = %s \n", f.Name())
	f.SetDeadline(time.Now())
}

func TestTmpDir(t *testing.T){
	wd, _ := os.Getwd()
	name, _ := ioutil.TempDir(wd, "tmpdir")
	fmt.Printf("tmpfile = %s \n",name)
}
