package fortest

import (
	"reflect"
	"sync"

	"forevernine.com/base/srvcore/framework"
)

type rspInfo struct {
	rsp     reflect.Type
	rspChan map[string]chan framework.IProto //key :local IP
}

type reqInfo struct {
	isLoad    int32
	wg        sync.WaitGroup
	fill      FillCall
	httpAfter HTTPAfterCall
	wsAfter   WSAfterCall
}

type FillCall func(iReq framework.IProto, ctx *framework.Context)
type HTTPAfterCall func(index int, iReq framework.IProto, iRsp framework.IProto, err error)
type WSAfterCall func(index int, iReq framework.IProto, err error)
