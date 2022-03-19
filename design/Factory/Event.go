package main

import "reflect"
import "fmt"

type Type uint8

// 事件类型定义
const (
	Start Type = iota
	End
)

// Event 事件抽象接口
type Event interface {
	EventType() Type
	Content() string
}

// StartEvent 开始事件，实现了Event接口
type StartEvent struct {
	content string
}

func (s *StartEvent) EventType() Type {
	return Start
}
func (s *StartEvent) Content() string {
	return reflect.TypeOf(*s).Name()
}

// EndEvent 结束事件，实现了Event接口
type EndEvent struct {
	content string
}

func (s *EndEvent) EventType() Type {
	return End
}
func (s *EndEvent) Content() string {
	return reflect.TypeOf(*s).Name()
}

// Factory 事件工厂对象
type Factory struct{}

// Create 根据事件类型创建具体事件
func (e *Factory) Create(eType Type) Event {
	switch eType {
	case Start:
		return &StartEvent{
			content: "this is start event",
		}
	case End:
		return &EndEvent{
			content: "this is end event",
		}
	default:
		return nil
	}
}

// OfStart Start类型Event的工厂方法
func OfStart() Event {
	return &StartEvent{
		content: "this is start event",
	}
}

// OfEnd End类型Event的工厂方法
func OfEnd() Event {
	return &EndEvent{
		content: "this is end event",
	}
}
func main() {
	factory := Factory{}
	e := factory.Create(Start)
	if e.EventType() != Start {
		fmt.Errorf("expect event.Start, but actual %v. ", e.EventType())
	}
	e = factory.Create(End)
	if e.EventType() != End {
		fmt.Errorf("expect event.End, but actual %v. ", e.EventType())
	}

	e = OfStart()
	if e.EventType() != Start {
		fmt.Errorf("expect event.Start, but actual %v. ", e.EventType())
	}
	e = OfEnd()
	if e.EventType() != End {
		fmt.Errorf("expect event.End, but actual %v. ", e.EventType())
	}
}
