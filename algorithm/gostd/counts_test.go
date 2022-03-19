package arr

import (
	"bufio"
	"fmt"
	"os"
	"testing"
)

func TestCounts(t *testing.T) {
	counts := make (map[string]int)
	var filePath string
	if(len(os.Args) >=2){
		filePath = os.Args[1]
	}else{
		fmt.Scanf("%s",&filePath)
	}
	f,err := os.Open(filePath)
	if err!=nil{
		fmt.Printf("open %s error = %v",filePath,err)
		os.Exit(0)
	}
	input := bufio.NewScanner(f)
	input.Split(bufio.ScanWords)
	/*for{
		var b bytes.Buffer
		var line []byte
		_,err=f.Read(line)
		if err != nil{
			break
		}
		b.Write(line)
		counts[b.String()]++
	}*/
	for input.Scan(){
		counts[input.Text()]++			//insert 2 map
	}
	f.Close()
	PrintMap(counts)
}
