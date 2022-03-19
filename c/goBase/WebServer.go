package main

import(
	"fmt"
	"log"
	"net/http"
	"sync"
	"image"
	"image/gif"
	"io"
	"math"
	"math/rand"
	"image/color"
)
var mutex sync.Mutex
var count int
func main(){
	http.HandleFunc("/",handler)			//call handler
	http.HandleFunc("/count",counter)
	handler:= func(w http.ResponseWriter,r *http.Request){			//std::function
		lissajous(w)
	}
	http.HandleFunc("/gif",handler)			//lambda
	
	log.Fatal(http.ListenAndServe("localhost:8000",nil))
}

func handler(w http.ResponseWriter,r* http.Request){
	mutex.Lock()
	count++
	mutex.Unlock()
	fmt.Fprintf(w,"URL.Path = %q \n",r.URL.Path)				//path /okok

	fmt.Fprintf(w,"%s %s %s \n",r.Method,r.URL,r.Proto)			//get/post  url  http1.1
	for k,v := range r.Header{									//http head
		fmt.Fprintf(w,"Handler [%q] = %q \n",k,v)
	}
	fmt.Fprintf(w,"Host = %q \n",r.Host)						//host addr
	fmt.Fprintf(w,"RemoteAddr = %q \n",r.RemoteAddr)			//client addr
	if err:= r.ParseForm();err!=nil{
		log.Print(err)
	}
	for k,v := range r.Form{									//htto form
		fmt.Fprintf(w,"Form [%q] = %q \n",k,v)
	}
}

func counter(w http.ResponseWriter,r* http.Request){
	mutex.Lock()
	fmt.Fprintf(w,"Count %d \n",count)
	mutex.Unlock()
}


func lissajous(out io.Writer){
	var palette = []color.Color{color.White,color.Black}
	const(
		whiteIndex = 0
		blackIndex = 1
	)
	const(
		cycles = 5
		res = 0.001
		size = 100
		nframes = 64
		delay = 8
	)	
	var freq = rand.Float64() * 3.0
	var anim = gif.GIF{LoopCount: nframes}
	var phase = 0.0
	for i := 0;i<nframes;i++{
		var rect = image.Rect(0,0,2*size +1,2*size + 1)
		img := image.NewPaletted(rect,palette)
		for t:= 0.0;t<cycles*2*math.Pi;t+=res{
			var x = math.Sin(t)
			var y = math.Sin(t*freq + phase)
			img.SetColorIndex(size + int(x*size + 0.5),size + int(y*size + 0.5),blackIndex)
		}
		phase+=0.1
		anim.Delay = append(anim.Delay,delay)
		anim.Image = append(anim.Image,img)
	}
	gif.EncodeAll(out,&anim)
}