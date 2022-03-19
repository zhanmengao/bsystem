package fortest

import (
	"forevernine.com/base/srvcore/framework"
	"sync"
)

var (
	reqMetaList = make(map[string]*reqInfo)
	rspMetaList = make(map[string]*rspInfo)
	ctxList []*framework.Context
	ctxLock sync.Mutex
)
