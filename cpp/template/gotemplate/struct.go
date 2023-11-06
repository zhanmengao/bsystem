package gotemplate

type TStruct[T int | string] struct {
	Name string
	Data T
}

type TTStruct[T int | float32, S []T] struct {
	Data     S
	MaxValue T
	MinValue T
}

//任何泛型类型都必须传入类型实参实例化才可以使用
var tts TTStruct[int, []int]
