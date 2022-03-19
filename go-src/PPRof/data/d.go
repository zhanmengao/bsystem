package data

import (
	"reflect"
	"unsafe"
)

var datas []string

func StrToBytes(str string) []byte {
	if len(str) == 0 {
		return nil
	}
	s := *(*reflect.StringHeader)(unsafe.Pointer(&str))
	b := &reflect.SliceHeader{Data: s.Data, Len: s.Len, Cap: s.Len}
	return *(*[]byte)(unsafe.Pointer(b))
}

// BtsToString  将Byte Slice转化成字符串
func BtsToString(bts []byte) string {
	if len(bts) == 0 {
		return ""
	}
	b := *(*reflect.SliceHeader)(unsafe.Pointer(&bts))
	s := &reflect.StringHeader{Data: b.Data, Len: b.Len}
	return *(*string)(unsafe.Pointer(s))
}

func Add(str string) string {
	data := StrToBytes(str)
	sData := BtsToString(data)
	datas = append(datas, sData)
	return sData
}
