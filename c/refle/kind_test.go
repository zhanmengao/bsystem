package refle

import (
	"bytes"
	"fmt"
	"reflect"
	"strconv"
	"testing"
	"time"
)

func TestKind(t *testing.T) {
	var x int64 = 1
	var d time.Duration = 1 * time.Nanosecond
	fmt.Println(formatAtom(reflect.ValueOf(x)))
	fmt.Println(formatAtom(reflect.ValueOf(d)))
	fmt.Println(formatAtom(reflect.ValueOf([]int64{x})))
	fmt.Println(formatAtom(reflect.ValueOf([]time.Duration{d})))
}

func formatAtom(v reflect.Value) string {
	switch v.Kind() {
	case reflect.Invalid:
		return "invalid"
	case reflect.Int, reflect.Int8, reflect.Int16,
		reflect.Int32, reflect.Int64:
		return strconv.FormatInt(v.Int(), 10)
	case reflect.Uint, reflect.Uint8, reflect.Uint16,
		reflect.Uint32, reflect.Uint64, reflect.Uintptr:
		return strconv.FormatUint(v.Uint(), 10)
	// ...floating-point and complex cases omitted for brevity...
	case reflect.Bool:
		return strconv.FormatBool(v.Bool())
	case reflect.String:
		return strconv.Quote(v.String())
	case reflect.Chan, reflect.Func, reflect.Ptr, reflect.Slice, reflect.Map:
		return v.Type().String() + " 0x" +
			strconv.FormatUint(uint64(v.Pointer()), 16)
	default: // reflect.Array, reflect.Struct, reflect.Interface
		return v.Type().String() + " value"
	}
}

func TestDisplay(t *testing.T) {
	strangelove := Movie{
		Title:    "Dr. Strangelove",
		Subtitle: "How I Learned to Stop Worrying and Love the Bomb",
		Year:     1964,
		Color:    false,
		Actor: map[string]string{
			"Dr. Strangelove":            "Peter Sellers",
			"Grp. Capt. Lionel Mandrake": "Peter Sellers",
			"Pres. Merkin Muffley":       "Peter Sellers",
			"Gen. Buck Turgidson":        "George C. Scott",
			"Brig. Gen. Jack D. Ripper":  "Sterling Hayden",
			`Maj. T.J. "King" Kong`:      "Slim Pickens",
		},

		Oscars: []string{
			"Best Actor (Nomin.)",
			"Best Adapted Screenplay (Nomin.)",
			"Best Director (Nomin.)",
			"Best Picture (Nomin.)",
		},
		cb: GetTime,
	}
	//!-strangelove
	fmt.Println(display("strangelove", reflect.ValueOf(strangelove)))
}

func display(path string, v reflect.Value) string {
	buf := bytes.Buffer{}
	switch v.Kind() {
	case reflect.Invalid:
		fmt.Fprintf(&buf, "%s = invalid \n", path)
	case reflect.Slice, reflect.Array:
		for i := 0; i < v.Len(); i++ {
			idxPath := fmt.Sprintf("%s[%d] \n", path, i)
			buf.WriteString(display(idxPath, v.Index(i))) //index 返回第i个元素
		}
	case reflect.Struct:
		for i := 0; i < v.NumField(); i++ {
			fieldPath := fmt.Sprintf("%s.%s \n", path, v.Type().Field(i).Name) //Field 返回第i个字段
			buf.WriteString(display(fieldPath, v.Field(i)))
		}
	case reflect.Map:
		for _, key := range v.MapKeys() { //MapKeys返回一个元素类型为reflect.Value的slice
			buf.WriteString(display(fmt.Sprintf("%s[%s] \n", path, formatAtom(key)), v.MapIndex(key))) //MapIndex返回key对应的值
		}
	case reflect.Ptr:
		if v.IsNil() {
			fmt.Fprintf(&buf, "%s == nil \n", path)
		} else {
			fmt.Fprintf(&buf, "%s.Type = %s \n", path, v.Elem().Type())
			buf.WriteString(display(path+".value", v.Elem())) //Elem返回指针指向的变量
		}
	case reflect.Interface:
		if v.IsNil() {
			fmt.Fprintf(&buf, "%s == nil \n", path)
		} else {
			fmt.Fprintf(&buf, "%s.type = %s \n", path, v.Elem().Type())
			buf.WriteString(display(path+".value", v.Elem())) //Elem获取动态值
		}
	default:
		fmt.Fprintf(&buf, "%s = %s \n", path, formatAtom(v))
	}
	return buf.String()
}

type Movie struct {
	Title, Subtitle string
	Year            int
	Color           bool
	Actor           map[string]string
	Oscars          []string
	Sequel          *string
	cb              func() int64
}
