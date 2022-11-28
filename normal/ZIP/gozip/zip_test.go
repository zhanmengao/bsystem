package main

import (
	"fmt"
	"os"
	"testing"
)

func TestZip(t *testing.T) {
	r, err := os.Open("../midplat_scheduler_servers_6_7ed927a177d8f502c309639ec49b4bcf.tar.gz")
	if err != nil {
		fmt.Println("error")
	}
	ExtractTarGz(r)
}
