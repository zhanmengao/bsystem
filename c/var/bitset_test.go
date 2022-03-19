package main

import(
	"fmt"
	"testing"
)

func TestBitset(t *testing.T) {
	var x uint8 = 1 << 1 | 1 << 5;
	var y uint8 = 1 << 1 | 1 << 2;
	fmt.Printf("%08b \n", x);					//{1,5}
	fmt.Printf("%08b \n", y);					//{1,2}
	fmt.Printf("%08b \n", x & y);				//{1}
	fmt.Printf("%08b \n", x | y);				//{1,2,5}
	fmt.Printf("%08b \n", x ^ y);				//{2,5}
	fmt.Printf("%08b \n", x &^ y);				//{5}

	for i := uint(0);i<8;i++{
		if x&(1<<i)!=0{
			fmt.Println(i)			//"1 5"
		}
	}
	fmt.Printf("%08b \n",x<<1)			//{2,6}
	fmt.Printf("%08b \n",x>>1)			//{0,4}
}