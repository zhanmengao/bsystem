package main

import (
	"net"
	"strconv"
	"strings"
	"time"
)

// 状态码值
const (
	TCP_ESTABLISHED = iota + 1
	TCP_SYN_SENT
	TCP_SYN_RECV
	TCP_FIN_WAIT1
	TCP_FIN_WAIT2
	TCP_TIME_WAIT
	TCP_CLOSE
	TCP_CLOSE_WAIT
	TCP_LAST_ACK
	TCP_LISTEN
	TCP_CLOSING
	//TCP_NEW_SYN_RECV
	//TCP_MAX_STATES
)

// 状态码
var states = map[int]string{
	TCP_ESTABLISHED: "ESTABLISHED",
	TCP_SYN_SENT:    "SYN_SENT",
	TCP_SYN_RECV:    "SYN_RECV",
	TCP_FIN_WAIT1:   "FIN_WAIT1",
	TCP_FIN_WAIT2:   "FIN_WAIT2",
	TCP_TIME_WAIT:   "TIME_WAIT",
	TCP_CLOSE:       "CLOSE",
	TCP_CLOSE_WAIT:  "CLOSE_WAIT",
	TCP_LAST_ACK:    "LAST_ACK",
	TCP_LISTEN:      "LISTEN",
	TCP_CLOSING:     "CLOSING",
	//TCP_NEW_SYN_RECV: "NEW_SYN_RECV",
	//TCP_MAX_STATES:   "MAX_STATES",
}

// socketEntry 结构体，用来存储/proc/net/tcp 每一行解析后数据信息
type socketEntry struct {
	id      int
	srcIP   net.IP
	srcPort int
	dstIP   net.IP
	dstPort int
	state   string

	txQueue       int
	rxQueue       int
	timer         int8
	timerDuration time.Duration
	rto           time.Duration // retransmission timeout
	uid           int
	uname         string
	timeout       time.Duration
	inode         string
}

// 解析/proc/net/tcp 行记录
func parseRawSocketEntry(entry string) (*socketEntry, error) {
	se := &socketEntry{}
	entrys := strings.Split(strings.TrimSpace(entry), " ")
	entryItems := make([]string, 0, 17)
	for _, ent := range entrys {
		if ent == "" {
			continue
		}
		entryItems = append(entryItems, ent)
	}

	id, err := strconv.Atoi(string(entryItems[0][:len(entryItems[0])-1]))
	if err != nil {
		return nil, err
	}
	se.id = id                                     // sockect entry id
	localAddr := strings.Split(entryItems[1], ":") // 本地 ip
	se.srcIP = parseHexBigEndianIPStr(localAddr[0])
	port, err := strconv.ParseInt(localAddr[1], 16, 32) // 本地 port
	if err != nil {
		return nil, err
	}
	se.srcPort = int(port)

	remoteAddr := strings.Split(entryItems[2], ":") // 远程 ip
	se.dstIP = parseHexBigEndianIPStr(remoteAddr[0])
	port, err = strconv.ParseInt(remoteAddr[1], 16, 32) // 远程 port
	if err != nil {
		return nil, err
	}
	se.dstPort = int(port)

	state, _ := strconv.ParseInt(entryItems[3], 16, 32) // socket 状态
	se.state = states[int(state)]

	tcpQueue := strings.Split(entryItems[4], ":")
	tQueue, err := strconv.ParseInt(tcpQueue[0], 16, 32) // 发送队列数据长度
	if err != nil {
		return nil, err
	}
	se.txQueue = int(tQueue)
	sQueue, err := strconv.ParseInt(tcpQueue[1], 16, 32) // 接收队列数据长度
	if err != nil {
		return nil, err
	}
	se.rxQueue = int(sQueue)

	se.uid, err = strconv.Atoi(entryItems[7]) // socket uid
	if err != nil {
		return nil, err
	}
	se.uname = entryItems[7] // socket user name
	se.inode = entryItems[9] // socket inode
	return se, nil
}

// hexIP 是网络字节序/大端法转换成的 16 进制的字符串
func parseHexBigEndianIPStr(hexIP string) net.IP {
	b := []byte(hexIP)
	for i, j := 1, len(b)-2; i < j; i, j = i+2, j-2 { // 反转字节，转换成小端法
		b[i], b[i-1], b[j], b[j+1] = b[j+1], b[j], b[i-1], b[i]
	}
	l, _ := strconv.ParseInt(string(b), 16, 64)
	return net.IPv4(byte(l>>24), byte(l>>16), byte(l>>8), byte(l))
}
