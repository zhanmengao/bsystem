// main.go
package main

import (
	"fmt"
	"log"
	"os"
	"path/filepath"

	"github.com/fsnotify/fsnotify"
)

//type MyWatcher *fsnotify.Watcher
func doev(watcher *fsnotify.Watcher, event fsnotify.Event) {
	switch event.Op {
	case fsnotify.Create:
		watcher.Add(event.Name)
		log.Println("create:", event.Name)
	case fsnotify.Rename, fsnotify.Remove:
		log.Println("remove:", event.Name)
		watcher.Remove(event.Name)
	case fsnotify.Write:
		log.Println("write:", event.Name)
	}
}
func main() {
	watchdir := os.Args[1]
	var err error
	watcher, err := fsnotify.NewWatcher()
	if err != nil {
		log.Fatal(err)
	}
	defer watcher.Close()

	done := make(chan bool)
	go func() {
		for {
			select {
			case event := <-watcher.Events:
				//log.Println("event:", event)
				doev(watcher, event)

			case err := <-watcher.Errors:
				log.Println("error:", err)
			}
		}
	}()
	err = watcher.Add(watchdir)
	if err != nil {
		log.Fatal(err)
	}
	err = filepath.Walk(watchdir, func(path string, info os.FileInfo, err error) error {
		err = watcher.Add(path)
		if err != nil {
			log.Fatal(err)
		}
		return nil
	})
	if err != nil {
		fmt.Printf("walk error [%v]\n", err)
	}
	<-done
}