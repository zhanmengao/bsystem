package tracing

import (
	"strings"

	"go.opentelemetry.io/otel"
	"go.opentelemetry.io/otel/exporters/jaeger"
	"go.opentelemetry.io/otel/sdk/resource"
	"go.opentelemetry.io/otel/sdk/trace"
	semconv "go.opentelemetry.io/otel/semconv/v1.4.0"
)

// InitJaeger initializes and registers jaeger to global TracerProvider.
//初始化Jaeger tracer
// The output parameter `tp` is used for waiting exported trace spans to be uploaded,
// which is useful if your program is ending and you do not want to lose recent spans.
//单进程的链路跟踪即进程内方法之间的调用链关系。这种场景的跟踪没有涉及到分布式跟踪，比较简单
func InitJaeger(serviceName, endpoint string) (tp *trace.TracerProvider, err error) {
	var endpointOption jaeger.EndpointOption
	if strings.HasPrefix(endpoint, "http") {
		// HTTP.
		endpointOption = jaeger.WithCollectorEndpoint(jaeger.WithEndpoint(endpoint))
	} else {
		// UDP.
		endpointOption = jaeger.WithAgentEndpoint(jaeger.WithAgentHost(endpoint))
	}

	// Create the Jaeger exporter
	exp, err := jaeger.New(endpointOption)
	if err != nil {
		return nil, err
	}
	tp = trace.NewTracerProvider(
		// Always be sure to batch in production.
		trace.WithBatcher(exp),
		// Record information about this application in an Resource.
		trace.WithResource(resource.NewWithAttributes(
			semconv.SchemaURL,
			semconv.ServiceNameKey.String(serviceName),
		)),
	)
	otel.SetTracerProvider(tp)
	return tp, nil
}
