package regex


import (
	"fmt"
	"regexp"
	"testing"
)
func TestDIV(t *testing.T) {
	// 原生字符串
	buf := `
    
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <title>C语言中文网 | Go语言入门教程</title>
</head>
<body>
    <div>Go语言简介</div>
    <div>Go语言基本语法
    Go语言变量的声明
    Go语言教程简明版
    </div>
    <div>Go语言容器</div>
    <div>Go语言函数</div>
</body>
</html>
    `
	//解释正则表达式
	reg := regexp.MustCompile(`<div>(?s:(.*?))</div>`)
	if reg == nil {
		fmt.Println("MustCompile err")
		return
	}
	//提取关键信息
	result := reg.FindAllStringSubmatch(buf, -1)
	//过滤<></>
	for _, text := range result {
		fmt.Println("text[1] = ", text[1])
	}
}