package goMutex
import "fmt"
import "sync"

func syncMutex(){

	var mutex sync.Mutex

	mutex.Lock()
	go func(){
		fmt.Println("mutex said: hello")
		mutex.Unlock()
	}()
	mutex.Lock()
	defer mutex.Unlock()
}

func syncChannel(){
	done := make(chan struct{})
	go func(){
		fmt.Println("channel said: hello")
		done<- struct{}{}
	}()
	<-done
}