package tm

import (
	"container/list"
	"math"
	"time"

	"forevernine.com/midplat/base_server/libs/safego"

	"forevernine.com/midplat/base_server/freequeue"

	"forevernine.com/midplat/base_server/libs/xlog"

	"forevernine.com/midplat/base_server/libs/xtime"
)

func getSleepTime(next int64) time.Duration {
	if next == math.MaxInt64 {
		return time.Duration(24) * time.Hour
	} else {
		now := xtime.Millisecond()
		cd := next - now
		ret := time.Duration(cd) * time.Millisecond
		return ret
	}
}
func Run() {
	onceList = list.New()
	wake = make(chan *timerJob, 1000)
	jobQueue = freequeue.NewFreeQueue(10)
	timer = time.NewTimer(time.Duration(24) * time.Hour)
	safego.Go(func() {
		for {
			select {
			//因为新插入任务而唤醒
			case tj := <-wake:
				next := doTimer(tj)
				timer.Reset(getSleepTime(next))
				//因为有任务到时间而唤醒
			case <-timer.C:
				next := doTimer(nil)
				timer.Reset(getSleepTime(next))
			}
		}
	})
}

//任务到时间
func doTimer(tj *timerJob) int64 {
	lock.Lock()
	defer lock.Unlock()
	if tj != nil {
		onceList.PushBack(tj)
	}
	var minTime int64 = math.MaxInt64
	now := xtime.Millisecond()
	//单次队列，调用完即删除
	for item := onceList.Front(); item != nil; {
		if job := item.Value.(*timerJob); job.nextCall < now {
			index++
			if err := jobQueue.Insert(index, func() {
				job.cb(job.ptr)
			}); err != nil {
				xlog.LevelLogfn(xlog.ERROR, "Push to job queue error ", err.Error())
			}
			delItem := item
			item = item.Next()
			onceList.Remove(delItem)
		} else if job.nextCall < minTime {
			minTime = job.nextCall
			item = item.Next()
		} else {
			item = item.Next()
		}

	}
	return minTime
}
