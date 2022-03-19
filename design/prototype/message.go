package main

import "fmt"
// Prototype 原型复制抽象接口
type Prototype interface {
	clone() Prototype
}
type Header struct {
	SrcAddr  string
	SrcPort  uint64
	DestAddr string
	DestPort uint64
	Items    map[string]string
}
type Body struct {
	Items []string
}
type Message struct {
	Header *Header
	Body   *Body
}

func (m *Message) clone() Prototype {
	msg := *m
	return &msg
}

func main() {
	msg := &Message{
		Header: &Header{
			DestAddr: "192.168.1.1",
			DestPort: 80,
			SrcAddr: "127.0.0.1",
			SrcPort: 49267,
		},
	}
	msgCpy := msg.clone()
	fmt.Printf("%v \n",msgCpy)
}
