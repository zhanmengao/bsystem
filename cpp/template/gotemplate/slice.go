package gotemplate

type IntSlice []int
type Slice[T int | float32 | float64] []T

var a Slice[int] = []int{1, 2, 3}

type TIntSlice Slice[int]
