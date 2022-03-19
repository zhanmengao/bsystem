package gstring

import (
	"fmt"
	"strings"
	"testing"
	"unicode"
)

// ToUpper 将 s 中的所有字符修改为其大写格式
// 对于非 ASCII 字符，它的大写格式需要查表转换
//func ToUpper(s string) string

// ToLower 将 s 中的所有字符修改为其小写格式
// 对于非 ASCII 字符，它的小写格式需要查表转换
//func ToLower(s string) string

// ToTitle 将 s 中的所有字符修改为其 Title 格式
// 大部分字符的 Title 格式就是其 Upper 格式
// 只有少数字符的 Title 格式是特殊字符
// 这里的 ToTitle 主要给 Title 函数调用
//func ToTitle(s string) string

func TestTo(t *testing.T) {
	s := "heLLo worLd Ａｂｃ"
	us := strings.ToUpper(s)
	ls := strings.ToLower(s)
	ts := strings.ToTitle(s)
	fmt.Printf("%q\n", us) // "HELLO WORLD ＡＢＣ"
	fmt.Printf("%q\n", ls) // "hello world ａｂｃ"
	fmt.Printf("%q\n", ts) // "HELLO WORLD ＡＢＣ"
}


// 获取非 ASCII 字符的 Title 格式列表

func TestToTitle(t *testing.T) {
	for _, cr := range unicode.CaseRanges {
		// u := uint32(cr.Delta[unicode.UpperCase]) // 大写格式
		// l := uint32(cr.Delta[unicode.LowerCase]) // 小写格式
		t := uint32(cr.Delta[unicode.TitleCase]) // Title 格式
		// if t != 0 && t != u {
		if t != 0 {
			for i := cr.Lo; i <= cr.Hi; i++ {
				fmt.Printf("%c -> %c\n", i, i+t)
			}
		}
	}
}



// ToUpperSpecial 将 s 中的所有字符修改为其大写格式。
// 优先使用 _case 中的规则进行转换
//func ToUpperSpecial(_case unicode.SpecialCase, s string) string

// ToLowerSpecial 将 s 中的所有字符修改为其小写格式。
// 优先使用 _case 中的规则进行转换
//func ToLowerSpecial(_case unicode.SpecialCase, s string) string

// ToTitleSpecial 将 s 中的所有字符修改为其 Title 格式。
// 优先使用 _case 中的规则进行转换
//func ToTitleSpecial(_case unicode.SpecialCase, s string) string

//_case 规则说明，以下列语句为例：
//unicode.CaseRange{'A', 'Z', [unicode.MaxCase]rune{3, -3, 0}}
//·其中 'A', 'Z' 表示此规则只影响 'A' 到 'Z' 之间的字符。
//·其中 [unicode.MaxCase]rune 数组表示：
//当使用 ToUpperSpecial 转换时，将字符的 Unicode 编码与第一个元素值（3）相加
//当使用 ToLowerSpecial 转换时，将字符的 Unicode 编码与第二个元素值（-3）相加
//当使用 ToTitleSpecial 转换时，将字符的 Unicode 编码与第三个元素值（0）相加

func TestToSpecial(t *testing.T) {
	// 定义转换规则
	var _MyCase = unicode.SpecialCase{
		// 将半角逗号替换为全角逗号，ToTitle 不处理
		unicode.CaseRange{',', ',',
			[unicode.MaxCase]rune{'，' - ',', '，' - ',', 0}},
		// 将半角句号替换为全角句号，ToTitle 不处理
		unicode.CaseRange{'.', '.',
			[unicode.MaxCase]rune{'。' - '.', '。' - '.', 0}},
		// 将 ABC 分别替换为全角的 ＡＢＣ、ａｂｃ，ToTitle 不处理
		unicode.CaseRange{'A', 'C',
			[unicode.MaxCase]rune{'Ａ' - 'A', 'ａ' - 'A', 0}},
	}
	s := "ABCDEF,abcdef."
	us := strings.ToUpperSpecial(_MyCase, s)
	fmt.Printf("%q\n", us) // "ＡＢＣDEF，ABCDEF。"
	ls := strings.ToLowerSpecial(_MyCase, s)
	fmt.Printf("%q\n", ls) // "ａｂｃdef，abcdef。"
	ts := strings.ToTitleSpecial(_MyCase, s)
	fmt.Printf("%q\n", ts) // "ABCDEF,ABCDEF."
}