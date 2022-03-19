package main

/*
# Copy
io的一个常用操作就是数据的复制，io包中提供了多个复制函数，直接将数据从Writer复制到Reader。
## Copy
Copy是最基础的复制函数，读取Writer中的数据，直到EOF，并写入Reader，函数声明如下：
func Copy(dst Writer, src Reader) (written int64, err error)

## CopyBuffer
CopyBuffer函数在Copy的基础上可以指定数据缓冲区。每次调用Copy函数时，都会生成一块临时的缓冲区，会带来一定的分配开销；CopyBuffer可以多次复用同一块缓冲区，其函数声明如下：
func CopyBuffer(dst Writer, src Reader, buf []byte) (written int64, err error)

## CopyN
CopyN在Copy的基础上，可以额外指定拷贝制定字节的数据，其函数声明如下：

func CopyN(dst Writer, src Reader, n int64) (written int64, err error)
 */