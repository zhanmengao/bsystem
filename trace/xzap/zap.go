package zap

import (
	"fmt"
	"go.uber.org/zap/zapcore"
	"ttrace/xlog"

	"go.uber.org/zap"
)

type Logger struct {
	log *zap.Logger
}

func DefaultZapConfig(dev bool) *zap.Logger {
	var zcfg zap.Config
	if dev {
		zcfg = zap.NewDevelopmentConfig()
	} else {
		zcfg = zap.NewProductionConfig()
	}

	zcfg.EncoderConfig.MessageKey = zapcore.OmitKey
	zcfg.EncoderConfig.EncodeTime = zapcore.RFC3339TimeEncoder
	zapl, err := zcfg.Build()
	if err != nil {
		return nil
	}
	return zapl
}

func NewZapLogger(zlog *zap.Logger) *Logger {
	return &Logger{zlog}
}

func (l *Logger) Log(level xlog.Level, keyvals ...interface{}) error {
	if len(keyvals) == 0 || len(keyvals)%2 != 0 {
		l.log.Warn(fmt.Sprint("Keyvalues must appear in pairs: ", keyvals))
		return nil
	}

	var data []zap.Field
	for i := 0; i < len(keyvals); i += 2 {
		data = append(data, zap.Any(fmt.Sprint(keyvals[i]), keyvals[i+1]))
	}

	switch level {
	case xlog.DEBUG:
		l.log.Debug("Debug => :", data...)
	case xlog.TRACE:
		l.log.Debug("Trace => : ", data...)
	case xlog.INFO:
		l.log.Info("Info => :", data...)
	case xlog.WARN:
		l.log.Warn("Warn => :", data...)
	case xlog.ERROR:
		l.log.Error("Error => :", data...)
	case xlog.FATAL:
		l.log.Fatal("Fatal => :", data...)
	}
	return nil
}

func (l *Logger) Write(data []byte) (n int, err error) {
	l.log.Info(string(data))
	n = len(data)
	return
}

func (l *Logger) Sync() error {
	return l.log.Sync()
}
