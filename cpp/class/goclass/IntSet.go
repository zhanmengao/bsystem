package class

import (
	"bytes"
	"fmt"
)
func SetBit(data *uint64,bitIdx uint,val bool){
	if val{
		*data |= 1<<(bitIdx-1)
	}else{
		*data &= ^(1<<bitIdx)
	}
}
func GetBit(data uint64,bitIdx uint)bool{
	return (data & (^(1<<bitIdx))) >0
}
type IntSet struct{
	words []uint64
}
func(s *IntSet)Add(x int){
	word,bit:=x/64,uint(x%64)						//计算在第几个字节，8位=64字节
	for word >=len(s.words){
		s.words = append(s.words,0)			//数组扩容
	}
	s.words[word] |= 1<<bit							//设置位
}
func(s *IntSet)Remove(x int){
	word,bit:=s.getWord(x)
	if word >= len(s.words){
		return
	}
	s.words[word] &= (^(1<<bit))
}
func(s* IntSet)Has(x int)bool{
	word,bit:=s.getWord(x)
	return word <len(s.words) && GetBit(s.words[word],bit)
}

func(s *IntSet)UnionWith(t* IntSet){
	for i,tword := range t.words{
		if i<len(s.words) {
			s.words[i] |= tword
		}else{
			s.words = append(s.words,tword)
		}
	}
}
func(s *IntSet)Len()uint{
	var count uint = 0
	for i:=0;i<len(s.words);i++{
		for j:=0;j<64;j++{
			if GetBit(s.words[i],uint(j)){
				count++
			}
		}
	}
	return count
}
func(s *IntSet)Clear(){
	s.words = make([]uint64,1)
}
func(s *IntSet)Copy()*IntSet{
	var ret IntSet
	copy(ret.words,s.words)
	return &ret
}
func(s *IntSet)getWord(x int)(word int,bit uint){
	word,bit =x/64,uint(x%64)
	return word,bit
}

func(s *IntSet)AddAll(argv ...int){
	for _,v := range argv{
		s.Add(v)
	}
}
func(s *IntSet)Data()(ret []uint64){
	copy(ret,s.words)
	return ret
}
func(s *IntSet)Elems()(ret []int){
	for i:=0;i<len(s.words);i++{
		for j :=0;j<64;j++{
			if GetBit(s.words[i],uint(j)){
				ret = append(ret,i*64 + j)
			}
		}
	}
	return ret
}
//实现Stringer接口
//强调方法
func(s *IntSet)String()string{
	var buf bytes.Buffer
	arr:=s.Elems()
	//fmt.Printf("[")
	buf.WriteString("[")
	for _,v := range arr{
		fmt.Fprintf(&buf,"%d ",v)
		//fmt.Printf("%d ",v)
	}
	//fmt.Printf("]\n")
	buf.WriteString("]\n")
	return buf.String()
}


