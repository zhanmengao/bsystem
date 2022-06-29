package main

import (
	"context"
	"go.opentelemetry.io/otel"
	"go.opentelemetry.io/otel/trace"
)

var tracer trace.Tracer

func init() {
	tracer = otel.Tracer("instrumentation/package/name")
}

func operation(ctx context.Context) {
	var span trace.Span
	ctx, span = tracer.Start(ctx, "operation")
	defer span.End()
	// ...
}
