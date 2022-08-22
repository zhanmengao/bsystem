package xlog

import (
	"bytes"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
)

type TEXTFormatter struct {
	pid         []byte
	skip        int
	logWithFunc bool
}

func NewTEXTFormatter() *TEXTFormatter {
	return &TEXTFormatter{pid: []byte(strconv.Itoa(os.Getpid())), skip: 3, logWithFunc: true}
}

func (t *TEXTFormatter) setSkip(skip int) {
	t.skip = skip
}

func (t *TEXTFormatter) setLogWithFunc(b bool) {
	t.logWithFunc = b
}

func (t *TEXTFormatter) Format(buf *bytes.Buffer, entry *Entry) (err error) {
	buf.Reset()
	buf.WriteString(time.Now().Format("[2006-01-02 15:04:05.000 -0700]"))
	buf.WriteString("\t")
	buf.WriteString(entry.level.String())
	buf.WriteString("\t")
	buf.Write(t.pid)
	buf.WriteString("\t")
	//
	if t.logWithFunc {
		pc, file, line, _ := runtime.Caller(t.skip)
		fun := runtime.FuncForPC(pc).Name()
		p := strings.LastIndex(file, "/")
		if p == -1 {
			p = 0
		} else {
			p++
		}

		buf.WriteString(file[p:])
		buf.WriteString(":")
		buf.WriteString(strconv.Itoa(line))
		buf.WriteString("\t")
		buf.WriteString("main." + fun[strings.LastIndex(fun, ".")+1:])
		buf.WriteString("\t")
	}
	buf.WriteString(entry.msg)
	buf.WriteString("\t")
	buf.WriteString("\n")
	//for _, t := range entry.ctx {
	//	switch vv := t.(type) {
	//	case string:
	//		buf.WriteString(vv)
	//	case []byte:
	//		buf.Write(vv)
	//	case int, uint, int8, uint8, int16, uint16, int32, uint32, int64, uint64:
	//		buf.WriteString(fmt.Sprintf("%d", vv))
	//	default:
	//		if v, ok := vv.(fmt.Stringer); ok {
	//			buf.WriteString(v.String())
	//		} else {
	//			continue
	//		}
	//	}
	//	buf.WriteByte(',')
	//}
	//修改符合日志规范
	//if len(entry.ctx) == 0 {
	//	buf.WriteByte('-')
	//} else {
	//	js, err := json.Marshal(entry.ctx)
	//	if err != nil {
	//		return fmt.Errorf("[logsdk]ctx to json failed: %s, data: %#v", err.Error(), entry.ctx)
	//	}
	//	buf.Write(js)
	//}
	//buf.WriteByte('\t')

	//if entry.mec != nil {
	//	// TODO
	//	js, err := json.Marshal(entry.mec)
	//	if err != nil {
	//		return fmt.Errorf("[logsdk]metric to json failed: %s, data: %#v", err.Error(), *(entry.mec))
	//	}
	//	buf.Write(js)
	//	buf.WriteString("\n")
	//} else {
	//	buf.WriteString("{}\n")
	//}

	return
}
