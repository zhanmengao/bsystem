package main

import (
	"bufio"
	"fmt"
	"strings"
	"testing"
)
/*
相较于io.Reader，bufio.Reader提供了很多实用的方法，能够更有效的对数据进行读取。首先是几个基础方法，它们能够对Reader进行细粒度的操作：

- Read，读取n个byte数据
- Discard，丢弃接下来n个byte数据
- Peek，获取当前缓冲区内接下来的n个byte，但是不移动指针
- Reset，清空整个缓冲区

其中前三个函数都没有参数，会从缓冲区读取一个满足需求的数据。
后面两个函数接收一个参数delim，用于做数据拆分，持续读取数据直到当前字节的值等于delim，然后返回这些数据；实际上这两个函数功能相同，只是在函数返回值的类型上有所区别。
func (b *Reader) ReadByte() (byte, error)

func (b *Reader) ReadRune() (r rune, size int, err error)

func (b *Reader) ReadLine() (line []byte, isPrefix bool, err error)

func (b *Reader) ReadBytes(delim byte) ([]byte, error)

func (b *Reader) ReadString(delim byte) (string, error)
 */

//使用ReadString方法获取用‘ ’分隔的字符串。
func TestReader(t *testing.T) {
	r := strings.NewReader("hello world !")
	reader := bufio.NewReader(r)

	for {
		str, err := reader.ReadString(byte(' '))
		fmt.Println(str)
		if err != nil {
			return
		}
	}
}

