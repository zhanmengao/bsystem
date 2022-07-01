package xlog

import (
	"fmt"
	"log"
)

var _ log.Logger

func (l *Logger) Log(level Level, keyvals ...interface{}) error {
	if len(keyvals) == 0 || len(keyvals)%2 != 0 {
		l.Warn(fmt.Sprint("Keyvalues must appear in pairs: ", keyvals))
		return nil
	}

	switch level {
	case TRACE:
		l.Trace(fmt.Sprint(keyvals...))
	case DEBUG:
		l.Debug(fmt.Sprint(keyvals...))
	case INFO:
		l.Info(fmt.Sprint(keyvals...))
	case NOTICE:
		l.Notice(fmt.Sprint(keyvals...))
	case WARN:
		l.Warn(fmt.Sprint(keyvals...))
	case ERROR:
		l.Error(fmt.Sprint(keyvals...))
	case FATAL:
		l.Fatal(fmt.Sprint(keyvals...))
	}
	return nil
}

func (l *Logger) Write(data []byte) (n int, err error) {
	l.Info(string(data))
	n = len(data)
	return
}
