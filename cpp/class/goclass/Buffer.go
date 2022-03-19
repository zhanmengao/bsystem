package class

import "bytes"
import "fmt"
import _"io"


var stdbuf bytes.Buffer
type Buffer struct{
	buf []byte
	initial [64]byte			//预分配的64字节
	offset int
}

func(b *Buffer)Grow(n int){
	if b.buf == nil{
		b.buf = b.initial[:0]
	}
	if len(b.buf) + n > cap(b.buf){
		buf := make([]byte,b.Len(),2*cap(b.buf)+ n)
		copy(buf,b.buf)
		b.buf = buf
	}
}
func(b *Buffer)Write(p []byte)(n int,err error){
	fmt.Printf("On write begin cap = %d len = %d \n",cap(b.buf),len(b.buf))
	b.Grow(len(p))
	b.buf = append(b.buf,p...)
	b.offset = len(b.buf)
	fmt.Printf("%s \n",b.buf)
	fmt.Printf("After write cap = %d len = %d \n",cap(b.buf),len(b.buf))
	return len(p),nil
}
func ReadFromString(s string)(*Buffer){
	var buf Buffer
	buf.Read([]byte(s))
	return &buf
}
func(b* Buffer)Read(p []byte)(n int,err error){
	copy(b.buf,p)
	b.offset = len(b.buf)
	fmt.Printf("After write cap = %d len = %d \n",cap(b.buf),len(b.buf))
	return len(p),nil
}
func(b *Buffer)Len()int{
	return b.offset
}

