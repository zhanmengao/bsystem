package safego

import (
	"bytes"
	"fmt"
	"log"
	"runtime/debug"
)

// Go safe function call
func Go(f func()) string{
	buf := &bytes.Buffer{}
	go func() {
		defer func() {
			if r := recover(); r != nil {
				log.Printf("[FATAL] panic occured> %v, stack:\n%s", r, string(debug.Stack()))

				fmt.Fprintf(buf,"%v", r)
				fmt.Fprintf(buf,"%s", string(debug.Stack()))
			}
		}()
		f()
	}()
	return buf.String()
}

func Do(f func()) string{
	buf := &bytes.Buffer{}
	func() {
		defer func() {
			if r := recover(); r != nil {
				log.Printf("[FATAL] panic occured> %v, stack:\n%s", r, string(debug.Stack()))
				fmt.Fprintf(buf,"%v", r)
				fmt.Fprintf(buf,"%s", string(debug.Stack()))
			}
		}()
		f()
	}()
	return buf.String()
}
