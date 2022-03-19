package gostb

import (
	"fmt"
	"testing"
)

func TestSort(t *testing.T) {
	var arr = []int{9,2,4,6,11,7,5}
	//Bubble(arr)
	QuickSort(arr,0,len(arr)-1)
	for i:= range arr{
		fmt.Printf("%d \n",arr[i])
	}
}
