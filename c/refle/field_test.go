package refle

import (
	"errors"
	"fmt"
	"reflect"
	"strconv"
	"strings"
	"testing"
)

type proto struct{
	str *string
}


func TestField(t *testing.T){

	var pb proto
	tp := reflect.TypeOf(pb)
	fmt.Println("type name = ",tp)

	v := reflect.New(tp)
	fmt.Println("type name = ",v.Type().String())

	fmt.Println(v.Elem().Field(0).Type().String())
}

func lookupField(field *reflect.StructField, tag string) bool {
	if tag == field.Name {
		return true
	}
	if name, ok := field.Tag.Lookup("json"); ok {
		arr := strings.Split(name, ",")
		if arr[0] == tag {
			return true
		}
	}
	return false
}

func setField(key, val string, ss reflect.Value) error {
	s := ss.Elem()
	t := s.Type()
	for i := 0; i < t.NumField(); i++ {
		ft := t.Field(i)
		fv := s.Field(i)
		if lookupField(&ft, key) {
			switch ft.Type.Kind() {
			case reflect.String:
				fv.SetString(val)
			case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64,
				reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64:
				intVal, err := strconv.Atoi(val)
				if err != nil {
					fmt.Printf("failed to parse interger param: %s", key)
					return err
				}
				fv.SetInt(int64(intVal))
			case reflect.Bool:
				boolVal, err := strconv.ParseBool(val)
				if err != nil {
					fmt.Printf("failed to parse bool param: %s", key)
					return err
				}
				fv.SetBool(boolVal)
			default:
				msg := fmt.Sprintf("unsupported param type:%s name:%s", ft.Type.Kind().String(), key)
				return errors.New(msg)
			}
			break
		}
	}
	return nil
}

func getFieldName(k []byte) string {
	if k[0] == '_' {
		k[0] = 'X'
		if len(k) >= 2 && k[1] >= 'a' && k[1] <= 'z' {
			// ascii letter to_upper_case, equals to k[0] -= 32
			k[1] &= ^byte(0x20)
		}
	} else if k[1] >= 'a' && k[1] <= 'z' {
		// ascii leeter to_upper_case, equals to k[0] -= 32
		k[0] &= ^byte(0x20)
	}
	return string(k)
}

