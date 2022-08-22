package main

import (
	"context"
	"github.com/opentracing/opentracing-go"
)

// FromProcess 夸进程传播
//
//使用上下文传递
func FromProcess(span opentracing.Span) {
	ctx := opentracing.ContextWithSpan(context.Background(), span)
	span, _ = opentracing.StartSpanFromContext(ctx, "req svc2")
	defer span.Finish()
}
