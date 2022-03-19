package arr

import (
	"fmt"
)

func PrintArrPoint(arr *[32]uint8){
	for i:=0;i<len(arr);i++{
		fmt.Printf("arr[%d] = %d \n",i,arr[i])
	}
}
func PrintArr(arr [32]uint8){
	for i:=0;i<len(arr);i++{
		fmt.Printf("arr[%d] = %d \n",i,arr[i])
	}
}