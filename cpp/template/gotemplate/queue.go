package gotemplate

type TQueue[T interface{}] struct {
	elements []T
}

// Put 将数据放入队列尾部
func (q *TQueue[T]) Put(value T) {
	q.elements = append(q.elements, value)
}

// Pop 从队列头部取出并从头部删除对应数据
func (q *TQueue[T]) Pop() (T, bool) {
	var value T
	if len(q.elements) == 0 {
		return value, true
	}

	value = q.elements[0]
	q.elements = q.elements[1:]
	return value, len(q.elements) == 0
}

// Size 队列大小
func (q *TQueue[T]) Size() int {
	return len(q.elements)
}
