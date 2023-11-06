package gotemplate

// Map
//KEY和VALUE是类型形参
//
//2、int|string 是KEY的类型约束， float32|float64 是VALUE的类型约束
//
//3、KEY int|string, VALUE float32|float64 整个一串文本因为定义了所有形参所以被称为类型形参列表
//
//4、Map[KEY, VALUE] 是泛型类型，类型的名字就叫 Map[KEY, VALUE]
//
//5、var a MyMap[string, float64] = xx 中的string和float64是类型实参，用于分别替换KEY和VALUE，实例化出了具体的类型 MyMap[string, float64]
type Map[KEY int | string, VALUE float32 | float64] map[KEY]VALUE
