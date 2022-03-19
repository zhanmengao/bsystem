package refle

import (
	"fmt"
	"math/rand"
	"reflect"
	"testing"
	"time"
)

type (
	product struct {
		id  int // 生产者序号
		val int // 产品
	}
	producer struct {
		id   int // 序号
		chnl chan *product
	}
)

var (
	producerList []*producer
	notifyNew    chan int
	updateDone   chan int
)
type selectType int
const(
	SelectNewProducer selectType = iota
	SelectTimer selectType = 1
	SelectProducer selectType = 2
)

func Test_Select(t *testing.T) {
	rand.Seed(time.Now().Unix())
	notifyNew = make(chan int, 1)
	updateDone = make(chan int, 1)
	ticker := time.NewTicker(time.Second)

	cases := update(ticker)

	for {
		chose, value, _ := reflect.Select(cases)
		switch selectType(chose) {
		case SelectNewProducer: // 有新的生产者
			cases = update(ticker)
			updateDone <- 1
		case SelectTimer:
			// 创建新的生产者
			if len(producerList) < 5 {
				go newProducer()
			}
		default:
			item := value.Interface().(*product)
			fmt.Printf("消费: 值=%d 生产者=%d\n", item.val, item.id)
		}
	}
}

func update(ticker *time.Ticker) (cases []reflect.SelectCase) {

	cases = make([]reflect.SelectCase,2)
	// 新生产者通知
	selectcase := reflect.SelectCase{
		Dir:  reflect.SelectRecv,
		Chan: reflect.ValueOf(notifyNew),
	}
	cases[SelectNewProducer] = selectcase

	// 定时器
	selectcase = reflect.SelectCase{
		Dir:  reflect.SelectRecv,
		Chan: reflect.ValueOf(ticker.C),
	}
	cases[SelectTimer] = selectcase

	// 每个生产者
	for _, item := range producerList {
		selectcase = reflect.SelectCase{
			Dir:  reflect.SelectRecv,
			Chan: reflect.ValueOf(item.chnl),
		}
		cases = append(cases, selectcase)
	}
	return
}

func newProducer() {
	newitem := &producer{
		id:   len(producerList) + 1,
		chnl: make(chan *product, 100),
	}
	producerList = append(producerList, newitem)
	notifyNew <- 1
	<-updateDone
	go newitem.run()
}

func (this *producer) run() {
	for {
		time.Sleep(time.Duration(int(time.Millisecond) * (rand.Intn(1000) + 1)))
		item := &product{
			id:  this.id,
			val: rand.Intn(1000),
		}
		fmt.Printf("生产: 值=%d 生产者=%d\n", item.val, item.id)
		this.chnl <- item
	}
}
