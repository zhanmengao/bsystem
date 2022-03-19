package fortest

import (
	"fmt"
	"forevernine.com/base/srvcore/config"
	"forevernine.com/base/srvcore/libs/xtime"
	"github.com/gogo/protobuf/proto"
	"github.com/gomodule/redigo/redis"
	"log"
	"reflect"
	"strconv"
	"strings"

	"forevernine.com/base/srvcore/datamgr"
)

// Redis "redis:user-midplat_global;passwd-edec43a8f72ebeb88950746db71eee59;servAddr-172.25.16.17:12100;package-forevernine.midplat.proto;cmd-scan 0 DeviceToken:test:* 500;type-string"
type Redis struct {
	val     string
	command string
	retType reflect.Type
	conn    redis.Conn
	tp      string

	pbPackage string

	passwd    string
	servAddr  string
	paramList map[string]string
}

func NewRedisMatch(v string) *Redis {
	v = strings.ReplaceAll(v, "redis:", "")
	ret := &Redis{
		paramList: map[string]string{},
	}
	ss := strings.Split(v, ";")
	for _, v := range ss {
		s2 := strings.Split(v, "-")
		switch s2[0] {
		case "cmd":
			ret.command = s2[1]
		case "passwd":
			ret.passwd = s2[1]
		case "servAddr":
			ret.servAddr = s2[1]
		case "package":
			ret.pbPackage = s2[1]
		case "type":
			//string
			ret.tp = s2[1]
			switch ret.tp {
			case "string":
				ret.retType = reflect.TypeOf("")
			case "int":
				ret.retType = reflect.TypeOf(1)
			case "float":
				ret.retType = reflect.TypeOf(0.1)
			default:
				//Dirty.Info.Content.string
				//pb结构体
				ret.retType = proto.MessageType(fmt.Sprintf("%s.%s", ret.pbPackage, s2[1]))
			}
		default:
			ret.paramList[strings.ToUpper(s2[0])] = s2[1]
		}
	}
	return ret
}

func (m *Redis) GetMaybeList() (strList []string) {
	var err error
	cfg := &config.DBConfig{
		Addrs:       []string{m.servAddr},
		Passwd:      m.passwd,
		TimeoutMsec: 500,
	}
	tm := xtime.Millisecond()
	if err = datamgr.AddRedisPool(strconv.Itoa(int(tm)), cfg); err != nil {
		log.Println(err)
		return
	}
	cli := datamgr.NewRedisClient(strconv.Itoa(int(tm)))
	var redisMethod iRedisMethod
	switch strings.ToLower(m.command) {
	case "scan":
		redisMethod = newRedisScan(cli)
	default:
		log.Printf("cmd %v not allow \n", m.command)
		return
	}
	if strList, err = redisMethod.Do(m.paramList); err != nil {
		log.Println(err)
		return
	}
	return
}
