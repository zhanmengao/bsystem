package main

import (
	opentracing "github.com/opentracing/opentracing-go"
	"github.com/opentracing/opentracing-go/ext"
	otlog "github.com/opentracing/opentracing-go/log"
	"github.com/uber/jaeger-client-go/config"
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/", test)
	http.ListenAndServe(":8000", nil)
}

func test(w http.ResponseWriter, r *http.Request) {
	log.Println(r.Header, r.URL)

	cfg, err := config.FromEnv()
	if err != nil {
		log.Println(err)
	}
	cfg.ServiceName = "svc2"
	cfg.Sampler = &config.SamplerConfig{
		Type:  "const",
		Param: 1,
	}
	tracer, closer, err := cfg.NewTracer()
	if err != nil {
		log.Println(err)
	}
	defer closer.Close()

	spanCtx, _ := tracer.Extract(opentracing.HTTPHeaders, opentracing.HTTPHeadersCarrier(r.Header))
	span := tracer.StartSpan("get haha", ext.RPCServerOption(spanCtx))
	defer span.Finish()
	log.Println(span.BaggageItem("haha"))

	span.LogFields(
		otlog.String("event", "string-format"),
		otlog.String("value", "hello wrold"),
	)
	w.Write([]byte("hello wrold"))
}
