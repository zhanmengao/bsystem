package fortest

import (
	"log"
	"reflect"
	"strconv"
	"sync"

	ws "github.com/gorilla/websocket"

	"forevernine.com/base/srvcore/framework"
)

type client struct {
	conn *ws.Conn
	lock sync.Mutex
}

func (c *client) readPacket() {
	var rsp framework.IProto
	defer c.conn.Close()
	for {
		if _, buf, err := c.conn.ReadMessage(); err != nil {
			log.Println(err)
			return
		} else {
			cmd, body := decodeCB(buf)
			if rspEle, exist := rspMetaList[strconv.Itoa(cmd)]; exist {
				rsp = reflect.New(rspEle.rsp).Interface().(framework.IProto)
				if err = rsp.Unmarshal(body); err != nil {
					log.Println(err)
					return
				}
				rspEle.rspChan[c.conn.LocalAddr().String()] <- rsp
				log.Printf("rcv %d . rsp : %s \n", cmd, rsp.String())
			} else {
				log.Println("data not found")
			}
		}
	}
}
