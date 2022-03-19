// +build !nacl
// run

// Copyright 2014 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Run the linkx test.

package main

import (
	"fmt"
	"os"
	"os/exec"
)

func main() {
	cmd := exec.Command("go", "run", "-ldflags=-X main.tbd hello -X main.overwrite trumped", "linkx.go")
	out, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Println(string(out))
		fmt.Println(err)
		os.Exit(1)
	}

	want := "hello\ntrumped\n"
	got := string(out)
	if got != want {
		fmt.Printf("got %q want %q\n", got, want)
		os.Exit(1)
	}
}
