package gostb

import (
	"hash/crc32"
	"sort"
	"strconv"
)

type Hash func(data []byte)uint32

type Map struct{
	hash Hash					//采取依赖注入的方式，允许用于替换成自定义的 Hash 函数，也方便测试时替换
	replicas int				//虚拟节点倍数 replicas
	keys []int					//哈希环 keys（虚拟节点的哈希值）：计算节点/机器的哈希值，放置在环上。
	hashMap map[int]string		//虚拟节点与真实节点的映射表 hashMap. k = 虚拟节点的哈希值,v = 真实节点的名称
}

func NewHashMap(replicas int,fn Hash)*Map{
	m:= &Map{
		replicas: replicas,
		hash: fn,
		hashMap: make(map[int]string),
	}
	if m.hash == nil{
		m.hash = crc32.ChecksumIEEE
	}
	return m
}


//Add 添加真实节点
func(m *Map)Add(keys ...string){
	//对每一个真实节点 key，对应创建 replicas 个虚拟节点
	for _,key := range keys{
		for i := 0;i<m.replicas;i++{
			hashCode := int(m.hash([]byte(strconv.Itoa(i))))			//虚拟节点的名称是：strconv.Itoa(i) + key，即通过添加编号的方式区分不同虚拟节点。
			m.keys = append(m.keys,hashCode)							//使用 m.hash() 计算虚拟节点的哈希值，使用 append(m.keys, hash) 添加到环上。
			m.hashMap[hashCode] = key									//在 hashMap 中增加虚拟节点和真实节点的映射关系。
		}
	}
	//环上的哈希值排序
	sort.Ints(m.keys)
}

func(m *Map)Get(key string)string{
	if len(m.keys) == 0{
		return ""
	}

	hashCode := int(m.hash([]byte(key)))

	//顺时针找到第一个匹配的虚拟节点的下标
	idx := sort.Search(len(m.keys),func(i int)bool{
		return m.keys[i] >= hashCode
	})

	//从 m.keys 中获取到对应的哈希值.因为 m.keys 是一个环状结构，所以用取余数的方式来处理这种情况。
	real := m.keys[idx % len(m.keys)]
	return m.hashMap[real]
}