package gotemplate

// ReadWriter 类型集：接口既有方法也有类型，所以是一般接口
//接口类型 ReadWriter 代表了一个类型集合，所有以 string 或 []rune 为底层类型，并且实现了 Read() Write() 这两个方法的类型都在 ReadWriter 代表的类型集当中。
type ReadWriter interface {
	~string | ~[]rune

	Read(p []byte) (n int, err error)
	Write(p []byte) (n int, err error)
}

type StringReadWriter string

func (s StringReadWriter) Read(p []byte) (n int, err error) {
	return
}
func (s StringReadWriter) Write(p []byte) (n int, err error) {
	return
}

type BytesReadWriter string

func (s BytesReadWriter) Read(p []byte) (n int, err error) {
	return
}
func (s BytesReadWriter) Write(p []byte) (n int, err error) {
	return
}

//接口定义中如果只有方法的话，那么这种接口被称为基本接口(Basic interface)。这种接口就是Go1.18之前的接口，用法也基本和Go1.18之前保持一致。
//如果接口内不光只有方法，还有类型的话，这种接口被称为 一般接口(General interface)

type MyError interface { // 接口中只有方法，所以是基本接口
	Error() string
}

type IUint interface { // 接口 Uint 中有类型，所以是一般接口
	~uint | ~uint8 | ~uint16 | ~uint32 | ~uint64
}
