package Interface

import (
	"sort"
	"testing"
)
import "fmt"
import "bytes"
type Player struct{
	rank int
	guid uint64
	name string
}
type PlayerList []Player
func(l PlayerList)Len()int{
	return len(l)
}
func(l PlayerList)Less(i,j int)bool{
	return l[i].rank > l[j].rank
}

func(l PlayerList)Swap(i,j int){
	temp := l[i]
	l[i] = l[j]
	l[j] = temp
}
func(l PlayerList)String()string{
	var buf bytes.Buffer
	for _,v := range l{
		fmt.Fprintf(&buf,"[%s] = [%d] \n",v.name,v.rank)
	}
	return buf.String()
}

type PlayerListEx struct{
	PlayerList
	less func(x,y *Player)bool
}
func(l PlayerListEx)Less(i,j int)bool{
	return l.less(&l.PlayerList[i],&l.PlayerList[j])
}
func LessEx(p1,p2 *Player)bool{
	if p1.rank > p2.rank{
		return true
	}else if p1.rank < p2.rank{
		return false
	}else{
		return p1.guid < p2.guid
	}
}
func Test_Sort(t *testing.T){
	var list PlayerList
	list = append(list,Player{rank: 2700,guid: 60,name: "weiliang"})
	list = append(list,Player{rank: 2700,guid: 3,name: "showmarker"})
	list = append(list,Player{rank: 2700,guid: 2,name: "faker"})

	list = append(list,Player{rank: 2600,guid: 107,name: "beimu"})
	list = append(list,Player{rank: 2500,guid: 100,name: "kejie"})
	list = append(list,Player{rank: 2400,guid: 97,name: "linger"})
	list = append(list,Player{rank: 1800,guid: 1,name: "wby"})

	fmt.Printf("is sort %v \n",sort.IsSorted(list))
	sort.Sort(list)
	//fmt.Printf("%s",list.String())
	sort.Reverse(list)
	fmt.Printf("%s \n",list.String())

	var listex PlayerListEx
	listex.PlayerList = list
	listex.less = LessEx
	sort.Sort(listex)
	fmt.Printf("%s \n",listex.String())
}


