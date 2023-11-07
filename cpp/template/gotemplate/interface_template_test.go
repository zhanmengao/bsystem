package gotemplate

import "testing"

// DataProcessor 泛型接口
type DataProcessor[T any] interface {
	Process(oriData T) (newData T)
	Save(data T) error
}
type DataProcessor2[T any] interface {
	int | ~struct{ Data interface{} }

	Process(data T) (newData T)
	Save(data T) error
}

type CSVProcessor struct {
}

func (c CSVProcessor) Process(oriData string) (newData string) {
	return
}

func (c CSVProcessor) Save(oriData string) error {
	return nil
}

func TestCSV(t *testing.T) {
	// 因为 T 的类型约束是 any，所以可以随便挑一个类型来当实参(比如string)：
	var processor DataProcessor[string] = CSVProcessor{}
	processor.Process("name,age\nbob,12\njack,30")
	_ = processor.Save("name,age\nbob,13\njack,31")

	// 错误。CSVProcessor没有实现接口 DataProcessor[int]
	//var processor2 DataProcessor[int] = CSVProcessor{}
}
