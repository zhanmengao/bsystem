package fortest

import (
	"log"
	"net/http"
	"reflect"
	"strconv"
	"sync"

	"forevernine.com/base/srvcore/framework"
	ws "github.com/gorilla/websocket"
)

var (
	wsConn     []*client
	decodeCB   func(pkt []byte) (cmd int, body []byte)
	encodeWSCB func(cmd int, proto framework.IProto, ctx *framework.Context) []byte
)

func RegisterWSEncode(cb func(cmd int, proto framework.IProto, ctx *framework.Context) []byte) {
	encodeWSCB = cb
}

func RegisterWSDecode(cb func(pkt []byte) (cmd int, body []byte)) {
	decodeCB = cb
}

func RegisterCMD(cmd int, rsp framework.IProto, fill FillCall, after WSAfterCall) {
	rspMetaList[strconv.Itoa(cmd+1)] = &rspInfo{
		rsp:     reflect.TypeOf(rsp).Elem(),
		rspChan: make(map[string]chan framework.IProto),
	}
	reqMetaList[strconv.Itoa(cmd)] = &reqInfo{
		isLoad:  0,
		wsAfter: after,
		fill:    fill,
	}
}

func Connect(sz int, addr string) {
	wg := &sync.WaitGroup{}
	wsConn = make([]*client, 0, sz)
	newContextList(sz)
	var lock sync.Mutex
	for i := 0; i < sz; i++ {
		wg.Add(1)
		go func(w *sync.WaitGroup, index int) {
			var cli *client
			call := func() error {
				defer w.Done()
				if c, _, err := ws.DefaultDialer.Dial(addr, http.Header{}); err == nil {
					cli = &client{
						conn: c,
					}

					lock.Lock()
					defer lock.Unlock()
					wsConn = append(wsConn, cli)
					ctxList[index] = framework.NewDefaultContext("")
					for _, rsp := range rspMetaList {
						rsp.rspChan[cli.conn.LocalAddr().String()] = make(chan framework.IProto, 10)
					}
					return nil
				} else {
					return err
				}
			}
			if err := call(); err != nil {
				log.Println(err)
			} else {
				cli.readPacket()
			}

		}(wg, i)
	}
	wg.Wait()
	log.Printf("connect success %d \n", len(wsConn))
}

func ReadPacket(index int, cmd int) (rsp framework.IProto) {
	c := wsConn[index]
	if rspCh, exist := rspMetaList[strconv.Itoa(cmd)]; exist {
		rsp = <-rspCh.rspChan[c.conn.LocalAddr().String()]
	}
	return
}
