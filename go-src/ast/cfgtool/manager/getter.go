package forevernine_com_config

import (
	"strconv"

)

type ABTestUser struct {
	UID string
}
type getUserInfoFunc func(data interface{}) *ABTestUser

var (
	getter [2]getUserInfoFunc //回调函数集,0为默认函数，1为注册进来的函数
)

func defaultGetter(data interface{}) *ABTestUser {
	switch vv := data.(type) {
	case string: //uid
		return getUserInfoByUID(vv)
	case *int64:
		s := strconv.FormatInt(*vv, 10)
		return getUserInfoByUID(s)
	}
	return nil
}

func getUserInfoByUID(uid string) *ABTestUser {
	return &ABTestUser{
		UID: uid,
	}
}
