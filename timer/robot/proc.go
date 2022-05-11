package tm

import (
	"time"

	"forevernine.com/midplat/base_server/libs/xtime"
)

func AddEvent(t time.Duration, cb func(data interface{}), ptr interface{}) {
	now := xtime.Millisecond()
	next := now + t.Milliseconds()
	job := &timerJob{
		cd:       t.Milliseconds(),
		nextCall: next,
		ptr:      ptr,
		cb:       cb,
	}
	wake <- job
}
