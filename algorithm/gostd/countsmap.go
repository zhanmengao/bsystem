package arr

import (
	"fmt"
)

func PrintMap(m map[string]int){
	for k,v := range m{
		fmt.Printf("%s : %d \n",k,v)
	}
}
