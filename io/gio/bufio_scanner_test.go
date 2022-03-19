package main

import (
	"bufio"
	"fmt"
	"strings"
	"testing"
)

/*实际使用中，更推荐使用Scanner对数据进行读取，而非直接使用Reader类。Scanner可以通过splitFunc将输入数据拆分为多个token，然后依次进行读取。

和Reader类似，Scanner需要绑定到某个io.Reader上，通过NewScannner进行创建，函数声明如下：

```
func NewScanner(r io.Reader) *Scanner
```

在使用之前还需要设置splitFunc（默认为ScanLines），splitFunc用于将输入数据拆分为多个token。bufio模块提供了几个默认splitFunc，能够满足大部分场景的需求，包括：

- ScanBytes，按照byte进行拆分
- ScanLines，按照行("\n")进行拆分
- ScanRunes，按照utf-8字符进行拆分
- ScanWords，按照单词(" ")进行拆分

通过Scanner的Split方法，可以为Scanner指定splitFunc。使用方法如下：

```
scanner := bufio.NewScanner(os.StdIn)

scanner.split(bufio.ScanWords）
```
除此了默认的splitFunc之外，也可以定义自己的splitFunc，函数需要满足如下声明：

```
type SplitFunc func(data []byte, atEOF bool) (advance int, token []byte, err error)
```
函数接收两个参数，第一个参数是输入数据，第二个参数是一个标识位，用于标识当前数据是否为结束。函数返回三个参数，第一个是本次split操作的指针偏移；第二个是当前读取到的token；第三个是返回的错误信息。

在完成了Scanner初始化之后，通过Scan方法可以在输入中向前读取一个token，读取成功返回True；使用Text和Bytes方法获取这个token，Text返回一个字符串，Bytes返回字节数组。方法声明如下：

```
func (s *Scanner) Scan() bool

func (s *Scanner) Text() string

func (s *Scanner) Text() []byte
*/


func TestScanner(t *testing.T) {

	scanner := bufio.NewScanner(strings.NewReader("hello world !"))

	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		fmt.Println(scanner.Text())
	}

}