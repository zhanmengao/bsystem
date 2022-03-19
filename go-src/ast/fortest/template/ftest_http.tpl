package fortest

import (
	"bufio"
	"encoding/json"
	"log"
	"os"

	pb "forevernine.com/midplat/server/internal/pbclass/auto_gen"
)

func Run{{ .Name }}Auto(url string, sz int) (err error){
	newContextList(sz)
	//生成pb随机文件
	file,err := makeReqFile(url,&pb.{{ .PbType }}{},sz)
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
	return Run{{ .Name }}ByList(url,sz, reqs)
}
func Run{{ .Name }}ByList(url string,sz int,reqList []*pb.{{ .PbType }}) (err error) {
	for i := 0; i < sz; i++ {
	    meta := reqMetaList[url]
	    meta.wg.Add(1)
		go func(index int) {
		    defer meta.wg.Done()
			req := reqList[index%len(reqList)]
			SendHTTPReq(url,index,req)
		}(i)
	}
	return
}