package main

import "fmt"

func main() {
	var slice []int = []int{9, 1, 5, 6, 8, 3, 7, 2, 10, 4}

	for i := 0; i < len(slice)-1; i++ {
		for j := 0; j < len(slice)-1-i; j++ {
			if slice[j] > slice[j+1] {
				slice[j], slice[j+1] = slice[j+1], slice[j]
			}
		}
	}

	fmt.Println(slice)
}
