package xlog

import (
	"bytes"
	"sync"
)

type Logger struct {
	level   Level
	wr      *Writer
	pool    *sync.Pool
	fmtr    *TEXTFormatter
	logHook func(uid string) bool
}

//const maxLogSize = 4096

func New(dir, app, name string, level Level, slice bool) (l *Logger, err error) {
	wr, err := NewWriter(app, dir, name, slice)
	if err != nil {
		return
	}
	pool := &sync.Pool{New: func() interface{} {
		buf := make([]byte, 0, 1024)
		bb := bytes.NewBuffer(buf)
		return bb
	}}
	l = &Logger{
		level: level,
		wr:    wr,
		pool:  pool,
		fmtr:  NewTEXTFormatter(),
	}
	InitOam()
	return
}

// msg: 日志内容, 一般清晰简短描述, 具体问题数据, 建议放在上下文数据ctx
// mod: 日志分类, 相同模块日志会聚集在一块, 作为文件名一部分
// mec: 日志基础数据, 作为定制告警或统计使用, 现阶段不需要那些功能, 允许传 `nil`
// ctx: 日志上下文数据, 作为错误、重要数据输出, 重现错误定位
func (l *Logger) Trace(msg string) {
	l.output(TRACE, msg)
}

func (l *Logger) Debug(msg string) {
	l.output(DEBUG, msg)
}

func (l *Logger) Info(msg string) {
	l.output(INFO, msg)
}

func (l *Logger) Notice(msg string) {
	l.output(NOTICE, msg)
}

func (l *Logger) Warn(msg string) {
	l.output(WARN, msg)
}

func (l *Logger) Error(msg string) {
	l.output(ERROR, msg)
}

// 调用Fatal, 程序讲退出
func (l *Logger) Fatal(msg string) {
	l.output(FATAL, msg)
	//os.Exit(1)
}

// 释放输出资源, 如: 文件描述符
func (l *Logger) Close() (err error) {
	l.wr.Close()
	return
}

func (l *Logger) SetSkip(skip int) {
	l.fmtr.setSkip(skip)
}

func (l *Logger) SetLogWithFunc(b bool) {
	l.fmtr.setLogWithFunc(b)
}

func (l *Logger) SetLevel(lv Level) {
	l.level = lv
}

// GetLevel get current level
func (l *Logger) GetLevel() Level {
	return l.level
}

//func (l *Logger) SetLogHook(f func(string) bool) {
//	l.logHook = f
//}

func (l *Logger) output(level Level, msg string) {
	if l.level <= level {
		entry := &Entry{
			level: level,
			msg:   msg,
			//mec:   mec,
			//ctx:   ctx,
			//mod: mod,
		}
		//if len(msg) > maxLogSize {
		//	entry.msg = msg[:maxLogSize]
		//}
		buf := l.pool.Get()
		defer l.pool.Put(buf)
		bb := buf.(*bytes.Buffer)
		l.fmtr.Format(bb, entry)
		data := bb.Bytes()
		l.wr.Write(data)
	}
}
