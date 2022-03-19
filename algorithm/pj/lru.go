package gostb
/*
   绿色的是字典(map)，存储键和值的映射关系。这样根据某个键(key)查找对应的值(value)的复杂是O(1)，在字典中插入一条记录的复杂度也是O(1)。
   红色的是双向链表(double linked list)实现的队列。将所有的值放到双向链表中，这样，当访问到某个值时，将其移动到队尾的复杂度是O(1)，在队尾新增一条记录以及删除一条记录的复杂度均为O(1)。
 */
import "container/list"

type Cache struct{
	maxBytes int64
	nbytes int64
	ll *list.List							//链表，表示其LRU值。队尾的LRU值高
	cache map[string]*list.Element			//map，存储节点，方便取值
	OnEvicted func(key string,value Value)
}

type entry struct{
	key string
	value Value
}

type Value interface {
	Len() int			//return sizeof(Value)
}

func NewLRUCache(maxBytes int64,onEvited func(string,Value))*Cache{
	return &Cache{
		maxBytes: 	maxBytes,
		ll:			list.New(),
		cache:		make(map[string]*list.Element),
		OnEvicted: 	onEvited,
	}
}

// Get 查找主要有 2 个步骤，第一步是从字典中找到对应的双向链表的节点，第二步，将该节点移动到队尾。
func(c *Cache)Get(key string)(val Value,ok bool){
	if ele,ok := c.cache[key];ok{					//如果键对应的链表节点存在，则将对应节点移动到队尾，并返回查找到的值。
		c.ll.MoveToFront(ele)						//c.ll.MoveToFront(ele)，即将链表中的节点 ele 移动到队尾（双向链表作为队列，队首队尾是相对的，在这里约定 front 为队尾）
		kv:=ele.Value.(*entry)
		return kv.value,true
	}
	return
}

// RemoveOldset 这里的删除，实际上是缓存淘汰。即移除最近最少访问的节点（队首）
func(c *Cache)RemoveOldset(){
	ele := c.ll.Back()								//c.ll.Back() 取到队首节点，从链表中删除。

	if ele!=nil{
		c.ll.Remove(ele)
		kv:=ele.Value.(entry)
		delete(c.cache,kv.key)												//从字典中 c.cache 删除该节点的映射关系。
		c.nbytes -=int64(len(kv.key))+ int64(kv.value.Len())				//更新当前所用的内存 c.nbytes。
		if c.OnEvicted!=nil{
			c.OnEvicted(kv.key,kv.value)									//如果回调函数 OnEvicted 不为 nil，则调用回调函数。
		}
	}
}



func(c *Cache)Add(key string,value Value){
	if ele,ok := c.cache[key];ok{
		c.ll.MoveToFront(ele)					//如果键存在，则更新对应节点的值，并将该节点移到队尾。
		kv := ele.Value.(*entry)
		c.nbytes += int64(value.Len()) - int64(kv.value.Len())
		kv.value = value
		//不存在则是新增场景，首先队尾添加新节点 &entry{key, value}, 并字典中添加 key 和节点的映射关系。
	}else{
		ele := c.ll.PushFront(&entry{key,value})
		c.cache[key] = ele
		c.nbytes += int64(len(key)) + int64(value.Len())
	}
	//更新 c.nbytes，如果超过了设定的最大值 c.maxBytes，则移除最少访问的节点。
	for c.maxBytes!=0 && c.maxBytes<c.nbytes{
		c.RemoveOldset()
	}
}

func(c *Cache)Len()int{
	return c.ll.Len()
}




