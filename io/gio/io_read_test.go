package main

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"testing"
)

var (
	r = strings.NewReader("some io.Reader stream to be read\n")
)

/*
# Reader
Reader对象是对输入设备的抽象，一个Reader可以绑定到一个输入对象，并在这个输入设备上读取数据，其声明如下：

```
type Reader interface {
    Read(p []byte) (n int, err error)
}
```
除了基础的Reader类之外，io包中还有LimitReader，MultiReader和TeeReader。
其中LimitReader只读取指定长度的数据；MultiReader用于聚合多个Reader，并依次进行读取；TeeReader将一个输入绑定到一个输出。具体声明如下：

```
func LimitReader(r Reader, n int64) Reader

func MultiReader(readers ...Reader) Reader

func TeeReader(r Reader, w Writer) Reader
```
这些衍生Reader都以包装的方式进行使用，也就是传入一个Reader，在这个Reader上增加额外功能，然后返回这个新Reader。*/
func TestLimitReader(t *testing.T) {
	buf := &bytes.Buffer{}
	r := strings.NewReader("some io.Reader stream to be read\n")
	lr := io.LimitReader(r, 4)
	io.Copy(buf, lr)
	fmt.Println(buf)
}

func TestMultiReader(t *testing.T) {
	buf := &bytes.Buffer{}
	r := strings.NewReader("LimitReader\n")
	r2 := strings.NewReader("MultiReader\n")
	r3 := strings.NewReader("TeeReader\n")
	lr := io.MultiReader(r, r2, r3)
	io.Copy(buf, lr)
	fmt.Println(buf)
}

//Go语言中的TeeReader()函数用于返回“Reader”，该“Reader”从声明的“r”读取，然后将其写入声明的“w”。然后，将通过声明的“r”执行的所有读取与对声明的“w”的等效写入进行比较。
//此方法不需要任何内部缓冲，并且读取完成后即可完成写入。
func TestTeeReader(t *testing.T) {
	var tee io.Reader
	w := &bytes.Buffer{}
	{
		reader := strings.NewReader("GfG\n")
		var buf bytes.Buffer
		tee = io.TeeReader(reader, &buf)
	}

	//使用Copy()方法返回Reader，从此处写入要读取的内容的位置使用了NewReader()字符串方法，并且在此还使用了外部缓冲区。
	Reader, err := io.Copy(w, tee)
	if err != nil {
		t.Fatal(err)
	}
	fmt.Printf("n:%v w = %v \n", Reader,w)
}

/*
## ReadAtLeast & ReadFull
这两个函数用于从Reader里面读取数据到指定缓冲区，ReadAtLeast会读取至少n个字节的数据，ReadFull会读取直到数据填满整个缓冲区。其函数声明如下：

```
func ReadAtLeast(r Reader, buf []byte, min int) (n int, err error)
func ReadFull(r Reader, buf []byte) (n int, err error)*/

func TestReadAtLeast(t *testing.T) {
	buf := make([]byte, 1024)
	io.ReadAtLeast(r, buf, 5)
	fmt.Println(string(buf))
}

func TestReadFull(t *testing.T) {
	buf := make([]byte, 16)
	io.ReadFull(r, buf)
	fmt.Println(string(buf))
}
