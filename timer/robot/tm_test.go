package tm

import (
	"log"
	"testing"
	"time"
)

var (
	count = 0
)

func onCB(ptr interface{}) {
	t := ptr.(*testing.T)
	count++
	if count <= 5 {
		log.Printf("add %d in %s ", count, time.Now().String())
		AddEvent(time.Duration(3)*time.Second, onCB, t)
	} else {
		t.Logf("ok")
	}
}

func TestOnce(t *testing.T) {
	Run()
	AddEvent(time.Duration(1)*time.Second, func(ptr interface{}) {
		log.Printf("1")
	}, t)
	t.Logf("add 1 ok")
	AddEvent(time.Duration(2)*time.Second, func(ptr interface{}) {
		log.Printf("2")
	}, t)
	t.Logf("add 2 ok")
	AddEvent(time.Duration(3)*time.Second, func(ptr interface{}) {
		log.Printf("3")
	}, t)
	t.Logf("add 3 ok")
}

func TestLoop(t *testing.T) {
	Run()
	AddEvent(time.Duration(2)*time.Second, onCB, t)
	select {}
}
