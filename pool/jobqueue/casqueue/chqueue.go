package JobQueue

import (
	"time"
)

type DataContainer struct {
	Queue chan interface{}
}

func NewDataContainer(cap int) (dc *DataContainer){
	dc = &DataContainer{}
	dc.Queue = make(chan interface{}, cap)
	return dc
}

// Push 非阻塞push
func (dc *DataContainer) Push(data interface{}, waittime time.Duration) bool{
	click := time.After(waittime)
	select {
	case dc.Queue <- data:
		return true
	case <- click:
		return false
	}
}

// Pop 非阻塞pop
func (dc *DataContainer) Pop(wait time.Duration) (data interface{}){
	click := time.After(wait)
	select {
	case data =<-dc.Queue:
		return data
	case <- click:
		return nil
	}
}
