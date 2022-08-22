package xlog

import (
	"bytes"
	"encoding/json"
	"fmt"
	"os"
	"path"
	"runtime"
	"strings"
	"sync"

	"github.com/gogo/protobuf/proto"
)

var (
	ossLogger *Logger
	ossLevel  Level
	// ErrLogger log for error message
	//ErrLogger *ErrorLogger

	consoleLogger *Logger

	procName string
)

type modString string

const (
	ModCommon modString = "xlog"
)

func DefaultConsoleLogger() *Logger {
	return consoleLogger
}

func init() {
	consoleLogger = &Logger{
		level: TRACE,
		wr: &Writer{
			app: "",
			dir: "",
			fileOp: &fileOp{
				fn: "",
				fp: os.Stdout,
			},
			chExit:  nil,
			RWMutex: sync.RWMutex{},
		},
		pool: &sync.Pool{New: func() interface{} {
			buf := make([]byte, 0, 1024)
			bb := bytes.NewBuffer(buf)
			return bb
		}},
		fmtr:    NewTEXTFormatter(),
		logHook: nil,
	}
	ossLogger = consoleLogger

}

func InitOam() {
}

func getLevelName(lv Level) string {
	for _, v := range logLevelName {
		if v.level == lv {
			return v.name
		}
	}
	return "trace"
}

// InitOssLogger init log
func InitOssLogger(baseDir, app string, lv string) (log *Logger, err error) {
	ossLevel = logLevelByName(lv)
	dir := path.Join(baseDir, app)
	ossLogger, err = New(dir, app, "", ossLevel, true)
	procName = app
	if err != nil {
		return
	}
	ossLogger.SetLogWithFunc(false)

	//ErrLogger = &ErrorLogger{w: ossLogger}
	log = ossLogger
	return
}

var logLevelName = []struct {
	name  string
	level Level
}{
	{"trace", TRACE},
	{"debug", DEBUG},
	{"notice", NOTICE},
	{"info", INFO},
	{"warn", WARN},
	{"error", ERROR},
	{"fatal", FATAL},
}

func logLevelByName(lv string) (level Level) {
	level = TRACE
	for _, v := range logLevelName {
		if lv == v.name {
			level = v.level
		}
	}
	return
}

func NameByLogLevel() (name string) {
	lv := ossLevel
	for _, v := range logLevelName {
		if lv == v.level {
			name = v.name
		}
	}
	return
}

// SetLevel sets log level
func SetLevel(lv string) {
	if lv == "" {
		return
	}
	ossLevel = logLevelByName(lv)
	ossLogger.SetLevel(ossLevel)
}

//func SetLogHook(f func(string) bool) {
//	ossLogger.SetLogHook(f)
//}

// LevelLogfn writes log with level
func LevelLogfn(level int, format string, param ...interface{}) {
	LevelLogfnSkip(3, level, format, param...)
}

// LevelLogfnMetric writes log with level and mertic
func LevelLogfnMetric(level int, format string, param ...interface{}) {
	LevelLogfnSkip(3, level, format, param...)
}

func LevelLogWithCtx(skip int, ctx interface{}, level int, format string, param ...interface{}) {
	lv := Level(level)
	if lv < ossLevel {
		return
	}

	loc := locFormat(skip)
	str := fmt.Sprintf("%s\t%s", loc, format)
	str = fmt.Sprintf(str, param...)

	if s, ok := ctx.(fmt.Stringer); ok {
		ctx = s.String()
	}

	if ossLogger == nil {
		buf, _ := json.Marshal(ctx)
		fmt.Println(str, string(buf))
		return
	}

	switch lv {
	case TRACE:
		ossLogger.Trace(str)
	case DEBUG:
		ossLogger.Debug(str)
	case ERROR:
		ossLogger.Error(str)
	case FATAL:
		ossLogger.Fatal(str)
	case INFO:
		ossLogger.Info(str)
	case WARN:
		ossLogger.Warn(str)
	case NOTICE:
		ossLogger.Notice(str)
	}

	return
}

func ToJson(v interface{}) string {
	js, _ := json.Marshal(v)
	return string(js)
}

func GetCurrentLV() int {
	return int(ossLevel)
}

// LevelLogfnSkip writes log with level and skip
func LevelLogfnSkip(skip int, level int, format string, param ...interface{}) {
	lv := Level(level)
	if lv < ossLevel {
		return
	}

	var arr []interface{}
	for _, v := range param {
		if _, ok := v.(proto.Message); ok {
			arr = append(arr, ToJson(v))
		} else {
			arr = append(arr, v)
		}
	}
	loc := locFormat(skip)
	str := fmt.Sprintf("%s\t%s", loc, format)
	str = fmt.Sprintf(str, arr...)

	if ossLogger == nil {
		fmt.Println(str)
		return
	}

	switch lv {
	case TRACE:
		ossLogger.Trace(str)
	case DEBUG:
		ossLogger.Debug(str)
	case ERROR:
		ossLogger.Error(str)
	case FATAL:
		ossLogger.Fatal(str)
	case INFO:
		ossLogger.Info(str)
	case WARN:
		ossLogger.Warn(str)
	case NOTICE:
		ossLogger.Notice(str)
	}

	return
}

// locFormat returns function name, file name and line number
func locFormat(skip int) string {
	pc, file, line, _ := runtime.Caller(skip)
	funcname := runtime.FuncForPC(pc).Name()
	index := strings.LastIndex(funcname, "/")

	return fmt.Sprintf("%s-%s:%d\t%s", procName, path.Base(file), line, funcname[index+1:])
}

type ErrorLogger struct {
	w *Logger
}

func (l *ErrorLogger) Write(buf []byte) (n int, err error) {
	l.w.Error("")
	return
}
