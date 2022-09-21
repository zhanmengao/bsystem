package pbpkg

import (
	"bufio"
	"os"
	"strings"
)

func GetPBPackage(tableProto string) (pkg string, err error) {
	pf, err := os.OpenFile(tableProto, os.O_RDONLY, 0x644)
	if err != nil {
		panic(err)
	}
	defer pf.Close()
	buf := bufio.NewReader(pf)
	for {
		line := ""
		line, err = buf.ReadString('\n')
		if err != nil {
			panic(err)
		}
		if strings.Contains(line, "go_package") {
			line = strings.TrimSpace(line)
			ss := strings.Split(line, "=")
			line = ss[1]
			line = strings.TrimSpace(line)
			line = strings.ReplaceAll(line, ";", "")
			line = strings.ReplaceAll(line, "\"", "")
			pkg = line
			return
		}
	}
	return
}
