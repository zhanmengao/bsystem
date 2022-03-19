package dirtyfilter

import (
	"fmt"
	"testing"
)

func TestFilter(t *testing.T) {
	f, err := NewFileDirtFilter("./dirty.ini")
	if err != nil {
		panic(err)
	}
	data, err := f.Filter("中华人民共和国，毛泽A东，江泽民，开车了，上去打飞机", ' ', '*')
	if err != nil {
		panic(err)
	}
	fmt.Println(data)
	ret, err := f.Replace("中华人民共和国，毛 泽东，江泽民，开车了，上去打飞机", '*')
	if err != nil {
		panic(err)
	}
	fmt.Println(ret)
	ret, err = f.Replace("毛abc  ,22!@.8*泽9fda,[东，江泽民，开车了，上去打飞机", '*')
	if err != nil {
		panic(err)
	}
	fmt.Println(ret)
	ret, err = f.Replace("fuck,,,f,uck..fauck", '*')
	if err != nil {
		panic(err)
	}
	fmt.Println(ret)
}

func BenchmarkNewFileDirtFilter(b *testing.B) {
	f, err := NewFileDirtFilter("./dirty.ini")
	if err != nil {
		panic(err)
	}
	for i := 0; i < b.N; i++ {
		//data, err := f.Filter("中华人民共和国，毛泽 东，江泽民，开车了，上去打飞机", ' ', '*')
		data, err := f.Replace("中华人民共和国，毛泽 东，江泽民，开车了，上去打飞机", '*')
		if err != nil {
			panic(err)
		}
		_ = data
	}

}