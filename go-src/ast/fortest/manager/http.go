package fortest

import (
	"bytes"
	"encoding/json"
	"errors"
	"forevernine.com/base/srvcore/libs/zerocopy"
	"forevernine.com/midplat/server/common/xerrors"
	"io/ioutil"
	"log"
	"net/http"
	"reflect"
	"sync"
	"time"

	"forevernine.com/base/srvcore/framework"
)

var (
	httpCli      *http.Client
	once         sync.Once
	timeout      = time.Duration(3) * time.Second
	encodeHTTPCB func(url string, proto framework.IProto, ctx *framework.Context) (realURL string, head map[string]string, body []byte)
)

func RegisterURL(url string, rsp framework.IProto, fill FillCall, after HTTPAfterCall) {
	rspMetaList[url] = &rspInfo{
		rsp:     reflect.TypeOf(rsp).Elem(),
		rspChan: make(map[string]chan framework.IProto),
	}
	reqMetaList[url] = &reqInfo{
		isLoad:    0,
		httpAfter: after,
		fill:      fill,
	}
}

func RegisterHTTPEncode(cb func(url string, proto framework.IProto, ctx *framework.Context) (realURL string, head map[string]string, body []byte)) {
	encodeHTTPCB = cb
}

func SetTimeout(to int64) {
	if to < 1 {
		return
	}
	timeout = time.Duration(to) * time.Second
	if httpCli != nil {
		httpCli.Timeout = timeout
	}
}

func newContextList(sz int) {
	ctxLock.Lock()
	defer ctxLock.Unlock()
	if len(ctxList) <= 0 {
		ctxList = make([]*framework.Context, 0, sz)
	}
	for i := len(ctxList); i < sz; i++ {
		ctxList = append(ctxList, framework.NewDefaultContext(""))
	}
}

func GetHttpClient() *http.Client {
	once.Do(func() {
		httpCli = &http.Client{
			Timeout: timeout * time.Second,
			Transport: &http.Transport{
				MaxIdleConns:        800,
				MaxIdleConnsPerHost: 800,
				DisableKeepAlives:   false,
				IdleConnTimeout:     timeout * 2 * time.Second,
			},
		}
	})
	return httpCli
}

func SendHTTPReq(url string, index int, req framework.IProto) {
	meta := reqMetaList[url]
	ctx := ctxList[index]
	fill := meta.fill
	if fill != nil {
		fill(req, ctx)
	}
	readURL, head, pkt := encodeHTTPCB(url, req, ctx)
	var rsp framework.IProto
	var rspBuf []byte
	var err error
	if rspBuf, err = sendHTTPReq(http.MethodPost, readURL, false, pkt, head); err != nil {
	} else {
		if info, exist := rspMetaList[url]; exist {
			rsp = reflect.New(info.rsp).Interface().(framework.IProto)
			if err = json.Unmarshal(rspBuf, rsp); err != nil {
				err = xerrors.ErrServerDecode().SetBasicErr(err).AppendText(zerocopy.BtsToString(rspBuf))
			}
		}
	}
	after := reqMetaList[url].httpAfter
	if after != nil {
		after(index, req, rsp, err)
	}
	return
}

func sendHTTPReq(method, url string, needLog bool, body []byte, headers ...map[string]string) (rspbuf []byte, err error) {
	if method == http.MethodPost && (body == nil || len(body) == 0) {
		err = errors.New("post body is nil")
		return
	}

	r := &http.Request{}
	switch method {
	case http.MethodGet:
		r, err = http.NewRequest(method, url, nil)
		if err != nil {
			return
		}
	case http.MethodPost:
		buf := bytes.NewBuffer(body)
		r, err = http.NewRequest(method, url, buf)
		if err != nil {
			return
		}
		if r.Header.Get("Content-Type") == "" {
			r.Header.Add("Content-Type", "application/json")
		}
	}

	if len(headers) > 0 {
		for _, h := range headers {
			for k, v := range h {
				r.Header.Set(k, v)
			}
		}
	}
	cli := GetHttpClient()
	rsp, err := cli.Do(r)
	if err != nil {
		return
	}
	defer rsp.Body.Close()

	rspbuf, err = ioutil.ReadAll(rsp.Body)
	if err != nil {
		return
	}

	if needLog {
		log.Printf("remote server response: %s \n", string(rspbuf))
	}
	return
}
