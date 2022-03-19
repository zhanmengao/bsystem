package gostb
import(
	"fmt"
)
func Swap(a *int,b *int){
	var temp int = *a
	*a = *b
	*b = temp
}

func Bubble(arr []int){
	for i:=0;i<len(arr);i++{
		for j:=i+1;j<len(arr);j++{
			if(arr[i]>arr[j]){
				Swap(&arr[i],&arr[j])	
			}
		}
	}
}

func QuickSort(arr []int,lo int,hi int){
	if hi - lo <=0{
		return
	}
	mid:=getIndex(arr,lo,hi)
	fmt.Printf("Quick : mid = %d begin = %d end = %d \n",mid,lo,hi)
	QuickSort(arr,lo,mid)
	QuickSort(arr,mid+1,hi)
}

func getIndex(arr []int,lo int,hi int)int{
	v:= arr[lo]
	idx:=lo
	for i:=lo+1;i<=hi;i++{
		if arr[i] < v{
			idx++
			Swap(&arr[i],&arr[idx])
		}
	}
	Swap(&arr[lo],&arr[idx])
	return idx
}

func main(){
	var arr = []int{9,2,4,6,11,7,5}
	//Bubble(arr)
	QuickSort(arr,0,len(arr)-1)
	for i:= range arr{
		fmt.Printf("%d \n",arr[i])
	}
}
