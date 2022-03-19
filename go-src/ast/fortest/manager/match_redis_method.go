package fortest

import (
	"forevernine.com/base/srvcore/datamgr"
	"strconv"
)

type iRedisMethod interface {
	Do(paramList map[string]string) (strList []string, err error)
}

type redisScan struct {
	cli *datamgr.RedisClient
}

func newRedisScan(cli *datamgr.RedisClient) *redisScan {
	return &redisScan{
		cli: cli,
	}
}

func (r *redisScan) Do(paramList map[string]string) (strList []string, err error) {
	var count = 500
	if str, exist := paramList["COUNT"]; exist {
		if count, err = strconv.Atoi(str); err != nil {
			count = 500
		}
	}
	err = r.cli.Scan(paramList["MATCH"], func(retList []string) bool {
		strList = append(strList, retList...)
		return len(strList) < count
	})
	return
}
