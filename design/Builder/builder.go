package main

import "sync"
import "fmt"

type Message struct {
	Header *Header
	Body   *Body
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

// Message对象的Builder对象
type builder struct {
	once *sync.Once
	msg *Message
}

// NewBuilder 返回Builder对象
func NewBuilder() *builder {
	return &builder{
		once: &sync.Once{},
		msg: &Message{
			Header: &Header{},
			Body: &Body{},
		},
	}
}

// WithSrcAddr 以下是对Message成员对构建方法
func (b *builder) WithSrcAddr(srcAddr string) *builder {
	b.msg.Header.SrcAddr = srcAddr
	return b
}
func (b *builder) WithSrcPort(srcPort uint64) *builder {
	b.msg.Header.SrcPort = srcPort
	return b
}
func (b *builder) WithDestAddr(destAddr string) *builder {
	b.msg.Header.DestAddr = destAddr
	return b
}
func (b *builder) WithDestPort(destPort uint64) *builder {
	b.msg.Header.DestPort = destPort
	return b
}
func (b *builder) WithHeaderItem(key, value string) *builder {
	// 保证map只初始化一次
	b.once.Do(func() {
		b.msg.Header.Items = make(map[string]string)
	})
	b.msg.Header.Items[key] = value
	return b
}
func (b *builder) WithBodyItem(record string) *builder {
	b.msg.Body.Items = append(b.msg.Body.Items, record)
	return b
}

// Build 创建Message对象，在最后一步调用
func (b *builder) Build() *Message {
	return b.msg
}

func main() {
	// 使用消息建造者进行对象创建
	message := NewBuilder().
		WithSrcAddr("192.168.0.1").
		WithSrcPort(1234).
		WithDestAddr("192.168.0.2").
		WithDestPort(8080).
		WithHeaderItem("contents", "application/json").
		WithBodyItem("record1").
		WithBodyItem("record2").
		Build()
	if message.Header.SrcAddr != "192.168.0.1" {
		fmt.Errorf("expect src address 192.168.0.1, but actual %s. ", message.Header.SrcAddr)
	}
	if message.Body.Items[0] != "record1" {
		fmt.Errorf("expect body item0 record1, but actual %s. ", message.Body.Items[0])
	}
}
