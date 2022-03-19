package main

import (
	"bufio"
	"bytes"
	"fmt"
	"log"
	"testing"
)

/*
# Writer
和Reader类似，Writer也对应的提供了多组方法。基础方法包括如下几个：

```
func (b *Writer) Write(p []byte) (nn int, err error) // 写入n byte数据

func (b *Writer) Reset(w io.Writer) // 重置当前缓冲区

func (b *Writer) Flush() error // 清空当前缓冲区，将数据写入输出
```

此外，Writer也提供了多个方法方便我们进行数据写入操作：

```
func (b *Writer) WriteByte(c byte) error  // 写入一个字节

func (b *Writer) WriteRune(r rune) (size int, err error） // 写入一个字符

func (b *Writer) WriteString(s string) (int, error) // 写入一个字符串
 */

func TestBufIoWrite(t *testing.T){
	buf := &bytes.Buffer{}
	writer := bufio.NewWriter(buf)
	_,err := writer.WriteString("Hello")
	if err!=nil{
		log.Fatal(err)
	}
	writer.WriteRune('?')
	writer.WriteByte('1')
	writer.Flush()
	fmt.Printf("buf = %s \n",buf.Bytes())
	writer.Write(buf.Bytes())
	writer.Flush()

	fmt.Printf("buf = %s \n",buf.Bytes())
}