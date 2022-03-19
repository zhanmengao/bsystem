package main

import "os"
import "text/template"

type person struct {
	Name string
	Age  int
}

func main(){
	p := person{
		"longshuai",
		23,
	}
	tmpl, _ := template.New("test").Parse("Name: {{.Name}}, Age: {{.Age}}")
	_ = tmpl.Execute(os.Stdout, p)
}