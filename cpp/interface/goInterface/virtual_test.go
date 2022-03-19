package Interface

import (
	"fmt"
	"testing"
)
import"strings"
import "bytes"
import "io"
import "os"


type ByteCounter struct{
	sz int
	line int
}

func (c  *ByteCounter)Write(p []byte)(int,error){
	c.sz += len(p)
	var sub []byte
	sub = append(sub,'\n')
	c.line += bytes.Count(p,sub)
	fmt.Printf("%s \ntotal sz = %d total line = %d \n",p,c.sz,c.line)
	return len(p),nil
}


func CountingWriter(w io.Writer)(io.Writer,*int64){
	var b ByteCounter
	var n int64 = int64(b.sz)
	return &b,&n
}

func Test_Virtual(t *testing.T){
	var b ByteCounter
	fmt.Fprintf(&b,"Hello world!\n?????")
	var w io.Writer
	w = os.Stdout
	w = new(bytes.Buffer)
	w = &b
	fmt.Fprintf(w,"Hello world!\n?????")

	r:=strings.NewReader("aabbcc")
	r.WriteTo(&b)
}