package trace_rpc

import (
	"context"
	"fmt"
	"go.opentelemetry.io/contrib/instrumentation/google.golang.org/grpc/otelgrpc"
	"go.opentelemetry.io/otel/baggage"
	"google.golang.org/grpc"
	"google.golang.org/grpc/metadata"
)

func getServer() *grpc.Server {
	server := grpc.NewServer(
		grpc.UnaryInterceptor(otelgrpc.UnaryServerInterceptor()),
		grpc.StreamInterceptor(otelgrpc.StreamServerInterceptor()),
	)
	return server
}

//On the server side, you can use gRPC metadata from an incoming request:
func getMeta(ctx context.Context) {
	if md, ok := metadata.FromIncomingContext(ctx); ok {
		fmt.Println(md)
	}
}

//Or use baggage API:
func getMetaBaggage(ctx context.Context) baggage.Baggage {
	bag := baggage.FromContext(ctx)
	return bag
}
