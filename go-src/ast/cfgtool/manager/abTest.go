package forevernine_com_config

import (
	"log"
	"runtime/debug"
)

const (
	tabArrayLen = 2
)

type AbTest struct {
	tab [tabArrayLen]interface{}
}

func (a *AbTest) register(t interface{}) {
	a.tab[0] = t
}

func (a *AbTest) getDefaultTable() interface{} {
	return a.tab[0]
}

func (a *AbTest) getTable(tabName string, data interface{}) interface{} {
	//默认返回
	if data == nil {
		log.Printf("GetTable %s data is nil, stack: %s", tabName, string(debug.Stack()))
		return a.tab[0]
	}

	//获取该用户对应的下标
	idx := getABIndex(tabName, data)

	return a.tab[idx]
}

func getABIndex(tabName string, data interface{}) int {
	info := getABUser(data)
	//默认返回
	if info == nil {
		log.Printf("GetTable %s info is nil %v", tabName, data)
		return 0
	}
	//获取到了用户信息，ABTest开始
	return 0
}

func getABUser(data interface{}) *ABTestUser {
	//遍历回调函数集
	var info *ABTestUser
	//从外部传入的开始调用
	for i := len(getter) - 1; i >= 0; i-- {
		f := getter[i]
		if f != nil {
			info = f(data)
			if info != nil {
				break
			}
		}
	}
	return info
}
