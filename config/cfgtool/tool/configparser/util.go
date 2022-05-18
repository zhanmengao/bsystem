package configparser

import (
	"bytes"
	"strings"
)

const (
	lower = false
	upper = true
)

// ToHump 小驼峰, 复制于网络, 如果需要用在实践项目需继续优化
func ToHump(name string) string {
	rst := ToBigHump(name)
	if len(rst) == 0 {
		return rst
	}

	ru := []rune(rst)
	if bool(ru[0] >= 'A' && ru[0] <= 'Z') { //首字母大写
		ru[0] += 32
	}

	return string(ru)
}

/*
	转换为大驼峰命名法则
	首字母大写，“_” 忽略后大写
*/
func ToBigHump(name string) string {
	if name == "" {
		return ""
	}

	temp := strings.Split(name, "_")
	var s string
	for _, v := range temp {
		vv := []rune(v)
		if len(vv) > 0 {
			if bool(vv[0] >= 'a' && vv[0] <= 'z') { //首字母大写
				vv[0] -= 32
			}
			s += string(vv)
		}
	}
	return s
}

// ToUnderline 回退网络模式
func ToUnderline(value string) string {
	if value == "" {
		return ""
	}

	var (
		buf                                      = bytes.NewBufferString("")
		lastCase, currCase, nextCase, nextNumber bool
	)

	for i, v := range value[:len(value)-1] {
		nextCase = bool(value[i+1] >= 'A' && value[i+1] <= 'Z')
		nextNumber = bool(value[i+1] >= '0' && value[i+1] <= '9')

		if i > 0 {
			if currCase == upper {
				if lastCase == upper && (nextCase == upper || nextNumber == upper) {
					buf.WriteRune(v)
				} else {
					if value[i-1] != '_' && value[i+1] != '_' {
						buf.WriteRune('_')
					}
					buf.WriteRune(v)
				}
			} else {
				buf.WriteRune(v)
				if i == len(value)-2 && (nextCase == upper && nextNumber == lower) {
					buf.WriteRune('_')
				}
			}
		} else {
			currCase = upper
			buf.WriteRune(v)
		}
		lastCase = currCase
		currCase = nextCase
	}

	buf.WriteByte(value[len(value)-1])

	s := strings.ToLower(buf.String())
	return s
}
