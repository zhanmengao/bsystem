package tm

type TimerType int32

type timerJob struct {
	ptr      interface{}
	cb       func(ptr interface{})
	nextCall int64
	cd       int64
}
