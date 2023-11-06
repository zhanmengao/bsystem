package gotemplate

import "testing"

/*
通过泛型receiver，泛型的实用性一下子得到了巨大的扩展。在没有泛型之前如果想实现通用的数据结构，诸如：堆、栈、队列、链表之类的话，我们的选择只有两个：
为每种类型写一个实现
1、使用 接口+反射
2、而有了泛型之后，我们就能非常简单地创建通用数据结构了。
*/
func TestSum(t *testing.T) {
	var s1 TSlice[int] = []int{1, 2, 3, 4}
	t.Logf("s1sum:%d \n", s1.Sum())

	var s2 TSlice[float32] = []float32{1.0, 2.0, 3.5, 4.2}
	t.Logf("s2sum:%f \n", s2.Sum())
}
