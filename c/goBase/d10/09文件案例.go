package main

import (
	"os"
	"fmt"
	"bufio"
	"io"
)

func main() {
	fp, err := os.Open("D:/dict.txt")
	if err != nil {
		fmt.Println("文件打开失败")
		return
	}
	defer fp.Close()

	//创建缓冲区
	r := bufio.NewReader(fp)
	dict := make(map[string]string)
	for {
		//读取两行内容  单词和翻译
		word, _ := r.ReadBytes('\n')
		trans, err1 := r.ReadBytes('\n')
		//将单词和翻译做类型转换 格式化 放在map中

		//#abandoner\r\n
		//abandoner
		//Trans:n. 遗弃者;委付者\r\n
		//n. 遗弃者;委付者
		dict[string(word[1:len(word)-2])] = string(trans[6 : len(trans)-2])

		//到文件结尾  循环结束
		if err1 == io.EOF {
			break
		}
	}

	//fmt.Println(dict)

	//单词查找
	word := ""
	fmt.Println("请输入您要查找的单词")
	fmt.Scan(&word)

	//fmt.Println(dict[word])
	value, ok := dict[word]
	if ok {
		fmt.Println(value)
	} else {
		fmt.Println("未找到该单词的翻译")
	}

}
