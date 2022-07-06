package main

import (
	"context"
	"go.opentelemetry.io/otel"
	"go.opentelemetry.io/otel/trace"
)

const (
	name    = "instrumentation/package/name"
	version = "0.1.0"
)

type Instrumentation struct {
	tracer trace.Tracer
}

func NewInstrumentation(tp trace.TracerProvider) *Instrumentation {
	if tp == nil {
		tp = otel.GetTracerProvider()
	}
	return &Instrumentation{
		tracer: tp.Tracer(name, trace.WithInstrumentationVersion(version)),
	}
}

func operation(ctx context.Context, inst *Instrumentation) {
	var span trace.Span
	ctx, span = inst.tracer.Start(ctx, "operation")
	defer span.End()
	// ...
}
