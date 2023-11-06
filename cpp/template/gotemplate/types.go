package gotemplate

type Wow[T int | string] int //本质是int

// TSlice 泛型类型Slice
type TSlice[T int | string | float32 | float64] []T

// FloatSlice 基于TSlice定义了新的类型FloatSlice
type FloatSlice[T float32 | float64] TSlice[T]

// IntAndStringSlice 基于TSlice定于了String或int的类型
type IntAndStringSlice[T int | string] TSlice[T]

// TIIntSlice 基于IntAndStringSlice定于新的IntSlice
type TIIntSlice[T int] IntAndStringSlice[T]

// TSliceMap 基于Slice实现map slice
type TSliceMap[T int | string] map[string]TSlice[T]
type TSliceMap2[T TSlice[int] | TSlice[string]] map[string]T
