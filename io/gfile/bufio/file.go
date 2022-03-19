package main

import(
	"bufio"
	"fmt"
	"os"
)

func main()  {
	counts := make (map[string]int)
	files := os.Args[1:]		//recv argv
	if(len(files) == 0){
		countLines(os.Stdin,counts)
	}else{
		for _,arg :=range files{
			f,err := os.Open(arg)
			if(err != nil){
				fmt.Fprintf(os.Stderr,"dup2: %v \n",err)
			}
			countLines(f,counts)
			f.Close()
		}
	}

}

func countLines(f* os.File,counts map[string]int){
	input := bufio.NewScanner(f)
	for(input.Scan()){
		counts[input.Text()] ++;
		for line,n := range counts{
			if n> 1{
				fmt.Printf("%d\t %s \n",n,line)
			}
		}
	}
}