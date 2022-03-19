package main

/*
void go_myFunction();

typedef void (*myFunction_f)();

myFunction_f pMyFunction;
*/
import "C"
import "unsafe"

//export go_myFunction
func go_myFunction() {
	// ...
}

func SetupFp() {
	C.pMyFunction = (*[0]byte)(unsafe.Pointer(C.go_myFunction))
}

func main(){
	SetupFp()
}
