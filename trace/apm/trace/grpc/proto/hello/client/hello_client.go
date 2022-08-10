package main

import (
	"fmt"
	"log"
	"time"
	"ttrace/apm/trace/grpc/interceptor"
	"ttrace/apm/trace/grpc/proto/hello/proto"
	"ttrace/jjaeger/ftracer"

	grpcMiddleware "github.com/grpc-ecosystem/go-grpc-middleware"
	"golang.org/x/net/context"
	"google.golang.org/grpc"
)

func main() {
	// tracer
	tracer, closer, _ := ftracer.CreateTracer("gRPC-hello")
	defer closer.Close()

	ctx, cancel := context.WithTimeout(context.Background(), time.Second*5)
	defer cancel()
	// conn
	conn, err := grpc.DialContext(
		ctx,
		"localhost:50051",
		grpc.WithInsecure(),
		grpc.WithBlock(),
		grpc.WithUnaryInterceptor(
			grpcMiddleware.ChainUnaryClient(
				interceptor.ClientInterceptor(tracer),
			),
		),
	)
	if err != nil {
		fmt.Println("grpc conn err:", err)
		return
	}
	client := proto.NewHelloClient(conn)
	r, err := client.SayHello(context.Background(), &proto.HelloReq{Name: "xiaoming"})
	if err != nil {
		log.Fatalf("could not greet: %v", err)
	}
	log.Printf("Greeting: %s", r.Message)
}
