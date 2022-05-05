package main

import (
	"fmt"
)

func (c *pulp) set(count int) {
	c.count = count
}

func (c *pulp) get() int {
	return c.count
}

//这个代码的好处就在于代码松耦合，易维护，还易测试。如果我们现在更换需求了，需要20个船桨，直接s.pulp.set(20)就可以了。
func main() {
	p := NewPulp()
	s := NewShip(p)
	s.pulp.set(12)
	fmt.Println(s.pulp.get())
}
