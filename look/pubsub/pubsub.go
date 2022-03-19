package pubsub

import (
	"sync"
	"time"
)

type (
	subscriber chan interface{}         		//订阅者为一个管道
	topicFunc  func(v interface{}) bool 		// 主题为一个过滤器
)

// Publisher 发布者对象
type Publisher struct {
	sync.RWMutex                          		//读写锁
	buffer       int                      		//订阅队列的缓存大小
	timeout      time.Duration            		//发布超时时间
	subscriber   map[subscriber]topicFunc 		//订阅者信息集
}

// NewPublisher 构建一个发布者对象, 可以设置发布超时时间和缓存队列的长度
func NewPublisher(publishTimeout time.Duration, buffer int) *Publisher {
	return &Publisher{buffer: buffer, timeout: publishTimeout, subscriber: make(map[subscriber]topicFunc)}
}

// Subscribe  添加一个新的订阅者，订阅全部主题：过滤函数为空，不进行过滤，直接从管道接收
func (p *Publisher) Subscribe() chan interface{} {
	return p.SubscribeTopic(nil)
}

// SubscribeTopic 添加一个新的订阅者，订阅过滤器筛选后的主题：往订阅者集合里添加一个通道
func (p *Publisher) SubscribeTopic(topic topicFunc) chan interface{} {
	ch := make(chan interface{}, p.buffer)
	p.Lock()
	defer p.Unlock()
	p.subscriber[ch] = topic
	return ch
}

// Evict 退出订阅
func (p *Publisher) Evict(sub chan interface{}) {
	p.Lock()
	defer p.Unlock()

	delete(p.subscriber, sub)
	close(sub)
}

// Publish 发布一个主题，发给所有订阅者 ：往通道里写一条数据
func (p *Publisher) Publish(v interface{}) {
	p.Lock()
	defer p.Unlock()
	var wg sync.WaitGroup
	for sub, topic := range p.subscriber {
		wg.Add(1)
		go p.sendTopic(sub,topic,v,&wg)
	}
	wg.Wait()
}

// 发送主题，可以容忍一定的超时
func (p *Publisher) sendTopic(sub subscriber, topic topicFunc, v interface{}, wg *sync.WaitGroup) {
	defer wg.Done()
	if topic != nil && !topic(v) {
		return
	}
	select {
	case sub <- v:
	case <-time.After(p.timeout):
	}
}

// Close 关闭发布者对象，同时关闭所有的订阅者管道。
func(p *Publisher)Close(){
	p.Lock()
	defer p.Unlock()
	for sub:= range p.subscriber{
		delete(p.subscriber,sub)
		close(sub)
	}
}