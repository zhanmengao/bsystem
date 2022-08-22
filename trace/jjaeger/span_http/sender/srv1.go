package main

import (
	"context"
	"github.com/opentracing/opentracing-go"
	"github.com/opentracing/opentracing-go/ext"
	"github.com/uber/jaeger-client-go/config"
	"log"
	"net/http"
)

func main() {
	cfg, err := config.FromEnv()
	if err != nil {
		log.Println(err)
	}
	cfg.ServiceName = "svc1"
	cfg.Sampler = &config.SamplerConfig{
		Type:  "const",
		Param: 1,
	}
	tracer, closer, err := cfg.NewTracer()
	if err != nil {
		log.Println(err)
	}
	defer closer.Close()
	opentracing.SetGlobalTracer(tracer)

	span := tracer.StartSpan("say hello")
	span.SetTag("role", "root")
	span.LogKV("hello", "world")
	defer span.Finish()

	ctx := opentracing.ContextWithSpan(context.Background(), span)
	testchildspan(ctx)
}
func testchildspan(ctx context.Context) {
	url := "http://localhost:8000"
	client := &http.Client{}
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		log.Println(err)
	}

	span, _ := opentracing.StartSpanFromContext(ctx, "req svc2")
	defer span.Finish()
	span.SetTag("role", "childspan")
	span.SetBaggageItem("haha", "heihei")

	ext.SpanKindRPCClient.Set(span)
	ext.HTTPUrl.Set(span, url)
	ext.HTTPMethod.Set(span, "GET")
	span.Tracer().Inject(
		span.Context(),
		opentracing.HTTPHeaders,
		opentracing.HTTPHeadersCarrier(req.Header),
	)

	resp, err := client.Do(req)
	if err != nil {
		log.Println(err)
	}
	log.Println(resp.Status)
}
