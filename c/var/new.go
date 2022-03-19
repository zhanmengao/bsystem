package main

var global *int

func f() {
	var x int
	x = 1
	global = &x				//heap
}

func g() {
	y := new(int)
	*y = 1					//stack
}

