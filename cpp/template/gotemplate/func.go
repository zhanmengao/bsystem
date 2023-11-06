package gotemplate

// Sum 可以给泛型类型添加方法吗？答案自然是可以的
func (s TSlice[T]) Sum() T {
	var sum T
	for _, value := range s {
		sum += value
	}
	return sum
}

// Add 不支持泛型方法
//func (s TSlice[T]) Add[T int | float32 | float64](a T, b T) T {
//	return a + b
//}
