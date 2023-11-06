package gotemplate

type IInterface[T int | float32 | string] interface {
	Func(data T)
}
