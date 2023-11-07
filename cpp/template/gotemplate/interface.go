package gotemplate

type IInterface[T int | float32 | string] interface {
	Func(data T)
}

//接口和接口、接口和普通类型之间也是可以通过 | 进行组合：

type Int interface {
	int | int8 | int16 | int32 | int64
}
type Uint interface {
	uint | uint8 | uint16 | uint32
}
type Float interface {
	float32 | float64
}

// SliceElement 同时，在接口里也能直接组合其他接口，所以还可以像下面这样：
type SliceElement interface {
	Int | Uint | Float | string // 组合了三个接口类型并额外增加了一个 string 类型
}

type IntUintFloatSlice[T Int | Uint | Float] []T // 使用 '|' 将多个接口类型组合

// BaseInt 在类型约束中使用类似 ~int 这种写法的话，就代表着不光是 int ，所有以 int 为底层类型的类型也都可用于实例化。
//限制：使用 ~ 时有一定的限制： 后面的类型不能为接口，后面的类型必须为基本类型
type BaseInt interface {
	~int | ~int8 | ~int16 | ~int32 | ~int64
}
