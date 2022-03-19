// Code generated by '$0', DO NOT EDIT.

package main

import (
	"fmt"
	"os"
	"sort"
	"strings"

	pb "{{.PackageName}}"
)

const (
	prefix    = "ERR_CODE_"
	prefixLen = len(prefix)
)

var (
	headers = `// Code generated by go run build_error.go. DO NOT EDIT.
// source: error.proto

package xerrors

import (
	"forevernine.com/base/srvcore/libs/terror"
)

`
)

func main() {
	fp, err := os.OpenFile("tmp.auto.gen.go", os.O_CREATE|os.O_RDWR|os.O_TRUNC, 0644)
	if err != nil {
		panic(err)
	}
	defer fp.Close()
	fmt.Fprint(fp, headers)
	t := pb.ERR_CODE_name
	codes := make([]int, 0, len(t))
	for k := range t {
		if k == 0 {
			continue
		}
		codes = append(codes, int(k))
	}
	sort.Ints(codes)
	for _, k1 := range codes {
		k := int32(k1)
		v := t[k]
		code := int(k)
		msg := formatMsg(v)
		varName := formatVar(v)
		fmt.Fprintf(fp, "\tfunc %s() *terror.Terror{\n \t return terror.New(%d, \"%s\", nil)\n}\n\n", varName, code, msg)
	}
}

func formatVar(str string) string {
	str = strings.ReplaceAll(str, prefix, "ERR_")
	str = strings.ToLower(str)
	pieces := strings.Split(str, "_")
	for i, v := range pieces {
		pieces[i] = strings.ToUpper(v[:1]) + v[1:]
	}
	return strings.Join(pieces, "")
}

func formatMsg(str string) string {

	if strings.HasPrefix(str, prefix) {
		str = str[prefixLen:]
	}
	str = strings.ReplaceAll(str, "_", " ")
	str = strings.ToLower(str)
	return str
}
