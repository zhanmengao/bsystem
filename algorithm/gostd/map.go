package arr

import (
	"fmt"
	"unsafe"
)
func PrintVal(m map[int]string,k int){
	v1,ok:= m[k]
	if !ok{
		fmt.Printf("%d not in m! \n",k)
	}else{
		fmt.Printf("m[%d] = %s \n",k,v1)
	}
}

type hmap struct {
	count     int // # live cells == size of map.  Must be first (used by len() builtin)
	flags     uint8
	B         uint8  // log_2 of # of buckets (can hold up to loadFactor * 2^B items)
	noverflow uint16 // approximate number of overflow buckets; see incrnoverflow for details
	hash0     uint32 // hash seed

	buckets    unsafe.Pointer // array of 2^B Buckets. may be nil if count==0.
	oldbuckets unsafe.Pointer // previous bucket array of half the size, non-nil only when growing
	nevacuate  uintptr        // progress counter for evacuation (buckets less than this have been evacuated)

	//extra *mapextra // optional fields
}
