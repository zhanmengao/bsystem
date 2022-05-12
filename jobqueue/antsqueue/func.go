package antsqueue

import (
	"log"
	"runtime/debug"
)

func panicHandler(err interface{}) {
	log.Printf("%v panic = %s .\n ", err, debug.Stack())
}
