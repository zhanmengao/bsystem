package fortest

import (
	"bufio"
	"encoding/json"
	"log"
	"os"
	"strconv"

	"github.com/gorilla/websocket"

	pb "{{ .ProtoPackage }}"
)

func Run{{ .Name }}Auto(cmd int,sz int) (err error)  {
    newContextList(sz)
	//生成pb随机文件
	file,err := makeReqFile(strconv.Itoa(cmd),&pb.{{ .PbType }}{},sz)
	if err!=nil{
	    return
	}
	pf, err := os.Open(file)
	if err != nil {
		return
	}
	reqs := make([]*pb.{{ .PbType }}, 0, 10)
	input := bufio.NewScanner(pf)
	for input.Scan() {
		req := &pb.{{ .PbType }}{}
		if err = json.Unmarshal(input.Bytes(), req); err != nil {
			log.Println(err)
			continue
		}
		reqs = append(reqs, req)
	}
	return Run{{ .Name }}ByList(cmd,reqs)
}

func Run{{ .Name }}ByList(cmd int,reqList []*pb.{{ .PbType }}) (err error) {
	for i := 0; i < len(wsConn); i++ {
		go func(index int) {
			req := reqList[index%len(reqList)]
			cli := wsConn[index]
			meta := reqMetaList[strconv.Itoa(cmd)]
			meta.wg.Add(1)
			defer meta.wg.Done()
			fill := meta.fill
			if fill!=nil{
			    fill(req,ctxList[index])
			}
			pkt := encodeWSCB(cmd,req,ctxList[index])

			cli.lock.Lock()
			defer cli.lock.Unlock()
			err := cli.conn.WriteMessage(websocket.BinaryMessage, pkt)
			after := reqMetaList[strconv.Itoa(cmd)].wsAfter
			if after != nil {
				after(index, req, err)
			}
		}(i)
	}
	return
}