package main

import (
	"context"
	"log"
	"net"
	"ttrace/apm/trace/grpc/interceptor"
	"ttrace/apm/trace/grpc/proto/hello/proto"
	"ttrace/jjaeger/ftracer"

	grpcMiddleware "github.com/grpc-ecosystem/go-grpc-middleware"
	"google.golang.org/grpc"
)

type helloServer struct{}

func (s *helloServer) SayHello(ctx context.Context, in *proto.HelloReq) (*proto.HelloRep, error) {
	return &proto.HelloRep{Message: "Hello " + in.Name}, nil
}

func main() {
	lis, err := net.Listen("tcp", "50051")
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
	}
	tracer, closer, _ := ftracer.CreateTracer("gRPC-hello")
	defer closer.Close()
	// UnaryInterceptor
	s := grpc.NewServer(grpc.UnaryInterceptor(
		grpcMiddleware.ChainUnaryServer(
			interceptor.ServerInterceptor(tracer),
		),
	))
	proto.RegisterHelloServer(s, &helloServer{})
	if err := s.Serve(lis); err != nil {
		panic(err)
	}
}
