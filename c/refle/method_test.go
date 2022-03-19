package refle

import (
	"fmt"
	"log"
	"os"
	"reflect"
	"runtime"
	"strings"
	"sync"
	"testing"
	"time"
)

func TestReflect(t *testing.T) {
	var wg sync.WaitGroup
	typ := reflect.TypeOf(&wg)
	for i := 0; i < typ.NumMethod(); i++ {
		method := typ.Method(i)
		argv := make([]string, 0, method.Type.NumIn())
		returns := make([]string, 0, method.Type.NumOut())
		// j 从 1 开始，第 0 个入参是 wg 自己。
		for j := 1; j < method.Type.NumIn(); j++ {
			argv = append(argv, method.Type.In(j).Name())
		}
		for j := 0; j < method.Type.NumOut(); j++ {
			returns = append(returns, method.Type.Out(j).Name())
		}
		log.Printf("func (w *%s) %s(%s) %s",
			typ.Elem().Name(),
			method.Name,
			strings.Join(argv, ","),
			strings.Join(returns, ","))
	}
}

func TestMethods(t *testing.T) {
	var s = reflect.TypeOf(&os.Process{})
	//获取函数列表
	for i := 0; i < s.NumMethod(); i++ {
		method := s.Method(i)
		fmt.Printf("method[%d] = %v \n",i,method)
	}
}

func GetTime()int64{
	return time.Now().Unix()
}

func GetMillTime()int64{
	return time.Now().UnixNano()
}

type handler func()int64

func TestFuncPtr(t *testing.T){
	value := reflect.ValueOf(GetTime)
	fmt.Println(value.Kind())
	fmt.Println(value.Type())
	fmt.Println(value.CanAddr())

	if value.Type() != reflect.TypeOf(GetMillTime){
		t.Fail()
	}

	name := runtime.FuncForPC(reflect.ValueOf(GetTime).Pointer()).Name()				//func name
	fmt.Println("Name of function : " + name)

	pFunc := value.Interface().(func() int64)
	fmt.Println(pFunc())

	newValue := reflect.New(value.Type())
	fmt.Println(newValue.Type())
}