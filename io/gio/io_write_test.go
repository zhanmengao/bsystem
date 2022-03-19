package main
/*
# Writer
Writer对象是对输出设备的抽象，一个Writer可以绑定到一个输出对象，并在这个输出设备上写入数据，其声明如下：

```
type Writer interface {
    Write(p []byte) (n int, err error)
}
```
和Reader类似，Writer也有MultiWriter，可以同步输出到多个Writer，声明如下：

```
func MultiWriter(writers ...Writer) Writer
```
## WriteString
WriteString函数用于向某个Writer写入一个字符串，其声明如下：

```
func WriteString(w Writer, s string) (n int, err error)
```

# ReadWriter
整合了Reader和Writer，可以同时进行读取和写入操作，声明如下：

```
type ReadWriter interface {
        Reader
        Writer
}
 */