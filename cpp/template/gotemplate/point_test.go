package gotemplate

import (
	"log"
	"testing"
)

func PointFunc[T any, PT interface {
	*T
}](p PT) {
	if p != nil {
		log.Println("ptr")
	} else {
		log.Println("nil")
	}
}

type DBProto struct {
}

func TestPoint(t *testing.T) {
	db := &DBProto{}
	PointFunc(db)
	db = nil
	PointFunc(db)
}
