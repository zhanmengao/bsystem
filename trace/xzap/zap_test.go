package zap

import (
	"testing"
	"ttrace/xlog"
)

func TestNewZapLogger(t *testing.T) {
	zlog := NewZapLogger(DefaultZapConfig(true))
	zlog.Log(xlog.INFO, "key", "value")
	zlog.Log(xlog.INFO, "key %s", "value")
	zlog.Log(xlog.INFO, "key", "value")
	zlog.Log(xlog.INFO, "key", "value")
}
