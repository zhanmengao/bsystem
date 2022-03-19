package fortest

import (
	"bufio"
	"encoding/json"
	"errors"
	"fmt"
	"log"
	"math/rand"
	"os"
	"path/filepath"
	"reflect"
	"strconv"
	"strings"
	"sync/atomic"
	"time"

	"forevernine.com/base/srvcore/libs/zerocopy"
)

func makeReqFile(cmd string, iReq interface{}, sz int) (output string, err error) {
	typ := reflect.TypeOf(iReq).Elem()

	output = filepath.Join(os.TempDir(), "autogen_"+typ.Name()+".json")
	//多次调用，只能为该类型生成一次文件
	if atomic.CompareAndSwapInt32(&reqMetaList[cmd].isLoad, 0, 1) {
		if err = os.Remove(output); err != nil {
			log.Println(err)
		}
		reqMetaList[cmd].wg.Add(1)
		defer reqMetaList[cmd].wg.Done()
		//这里用interface。因为要递归处理结构体类型，所以数组里边可能继续夹数组
		meta := make([][]interface{}, typ.NumField())
		for i := 0; i < typ.NumField(); i++ {
			meta[i] = createMaybeList(typ.Field(i))
		}
		var pf *os.File
		//离谱的事情，如果把EXCL删了会报错
		pf, err = os.OpenFile(output, os.O_WRONLY|os.O_CREATE|os.O_EXCL, 0644)
		if err != nil {
			return
		}
		defer pf.Close()
		//写入文件时，使用带缓存的 *Writer
		write := bufio.NewWriter(pf)

		//生成sz个值
		for i := 0; i < sz; i++ {
			req := reflect.New(typ).Elem()
			makeReq(req, meta, i)
			//写文件
			jsonData, _ := json.Marshal(req.Addr().Interface())

			if _, err = write.Write(jsonData); err != nil {
				log.Println(err)
			} else {
				if _, err = write.WriteString("\n"); err != nil {
					log.Println(err)
				}
			}
		}
		write.Flush()
		return
	}
	reqMetaList[cmd].wg.Wait()
	return
}

func createMaybeList(typ reflect.StructField) []interface{} {
	switch typ.Type.Kind() {
	case reflect.Ptr:
		return createStructMaybe(typ.Type.Elem())
	default:
		return createBaseMaybe(typ)
	}
}

func createStructMaybe(typ reflect.Type) []interface{} {
	ret := make([]interface{}, typ.NumField())
	for i := 0; i < typ.NumField(); i++ {
		ret[i] = createMaybeList(typ.Field(i))
	}
	return ret
}

func createBaseMaybe(typ reflect.StructField) []interface{} {
	//读tag
	maybe := typ.Tag.Get("maybe")
	if maybe == "" {
		return nil
	}
	//解析可能的值
	ss := strings.Split(maybe, "|")
	if len(ss) <= 1 {
		ss = []string{
			maybe,
		}
	}
	//生成可能性列表
	dataList := make([]interface{}, 0)
	for _, v := range ss {
		match := getMatch(v)
		maybeList := match.GetMaybeList()
		for i, _ := range maybeList {
			dataList = append(dataList, maybeList[i])
		}
	}
	return dataList
}

func makeReq(req reflect.Value, meta [][]interface{}, index int) {
	for i := 0; i < req.NumField(); i++ {
		//该字段有没有可以写的值
		if len(meta[i]) <= 0 {
			continue
		}
		field := req.Field(i)
		switch field.Kind() {
		case reflect.Ptr:
			ele := reflect.New(field.Type().Elem())
			req.Field(i).Set(ele)
			makeStructVal(req.Field(i).Elem(), meta[i], index)
		default:
			makeBaseVal(req.Field(i), meta[i], index+i)
		}

	}
}

func makeStructVal(field reflect.Value, meta []interface{}, index int) {
	meta2 := make([][]interface{}, 0)
	for _, v := range meta {
		vv := v.([]interface{})
		meta2 = append(meta2, vv)
	}
	makeReq(field, meta2, index)
}

func makeBaseVal(field reflect.Value, meta []interface{}, index int) {
	rand.Seed(time.Now().UnixNano())
	index = (rand.Int() + index) % len(meta)
	if err := setField(field, meta[index].(string)); err != nil {
		log.Println(err)
	}
}

func setField(rv reflect.Value, val string) error {
	switch rv.Kind() {
	case reflect.String:
		rv.SetString(val)
	case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64,
		reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
		intVal, err := strconv.Atoi(val)
		if err != nil {
			return err
		}
		rv.SetInt(int64(intVal))
	case reflect.Bool:
		boolVal, err := strconv.ParseBool(val)
		if err != nil {
			return err
		}
		rv.SetBool(boolVal)
	case reflect.Slice:
		rv.SetBytes(zerocopy.StrToBytes(val))
	default:
		return errors.New(fmt.Sprintf("type %s not allow", rv.Type().String()))
	}
	return nil
}
