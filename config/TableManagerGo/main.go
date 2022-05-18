package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"strings"
)

type tabDataType int32

const (
	tabDataString tabDataType = iota
	tabDataInt32
	tabDataInt64
)

type tabData struct {
	name string
	typ  tabDataType
}

var table []tabData

func getPBType(t tabDataType) string {
	switch t {
	case tabDataString:
		return "string"
	case tabDataInt32:
		return "int32"
	case tabDataInt64:
		return "int64"
	}
	return ""
}

func main() {
	dir := os.Args[1]
	dst := os.Args[2]
	goPath := os.Args[3]
	infos,err := ioutil.ReadDir(dir)
	if err!=nil{
		panic(err)
	}
	//写pb文件
	pbFile, err := os.OpenFile(dst, os.O_CREATE|os.O_TRUNC|os.O_RDWR, 0644)
	if err != nil {
		panic(err)
	}
	defer pbFile.Close()
	pbFile.WriteString("syntax = \"proto3\";\n")
	pbFile.WriteString("package table;\n")
	//生成读写函数
	goFile, err := os.OpenFile(goPath, os.O_CREATE|os.O_TRUNC|os.O_RDWR, 0644)
	if err != nil {
		panic(err)
	}
	defer goFile.Close()
	//packet
	goFile.WriteString("package table\n")
	goFile.WriteString("//make by robot . don't edit!!!!\n")
	for _,v := range infos{
		file := v.Name()
		if !strings.HasSuffix(file,"txt"){
			continue
		}
		message := strings.SplitN(file, ".", 2)[0]
		//读文件，生成字段名
		sb := strings.Builder{}
		sb.WriteString(dir)
		sb.WriteRune('/')
		sb.WriteString(file)
		pf, err := os.Open(sb.String())
		if err != nil {
			panic(err)
		}

		rd := bufio.NewReader(pf)

		//读第一行，生成字段名
		line, err := rd.ReadString('\r')
		if err != nil {
			panic(err)
		}
		//按空格分割
		ss := strings.Split(line, "\t")
		table = make([]tabData, len(ss))
		for i, v := range ss {
			strings.Trim(v, " ")
			fmt.Printf("[%d] = %s \n", i, v)
			table[i] = tabData{
				name: v,
			}
		}

		//读第二行 生成字段类型
		line, err = rd.ReadString('\r')
		if err != nil {
			panic(err)
		}
		//按空格分割
		ss = strings.Split(line, "\t")
		for i, v := range ss {
			v = strings.ReplaceAll(v, "\n", "")
			fmt.Printf("[%d] = %s \n", i, v)
			switch v {
			case "STRING":
				table[i].typ = tabDataString
			case "INT32":
				table[i].typ = tabDataInt32
			case "INT64":
				table[i].typ = tabDataInt64
			}
		}
		pf.Close()

		//pb
		pbFile.WriteString(fmt.Sprintf("message %s {\n", message))
		for i, v := range table {
			pbFile.WriteString(fmt.Sprintf("%s %s  = %d; \n", getPBType(v.typ), v.name, i + 1))
		}
		pbFile.WriteString(fmt.Sprintf("}"))


		goFile.WriteString(fmt.Sprintf("var %s map[int32]*table.%s\n", strings.ToLower(message), message))
		//GetByID
		goFile.WriteString(fmt.Sprintf("func Get%sByID(id int32)*table.%s{\n", message, message))
		goFile.WriteString(fmt.Sprintf("v,ok := %s[id]\n", strings.ToLower(message)))
		goFile.WriteString("if ok{\nreturn v\n}")
		goFile.WriteString("else{\nreturn nil\n}\n")
		goFile.WriteString("}")
	}

}
