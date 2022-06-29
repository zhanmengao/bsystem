package main

import (
	"context"

	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/gtrace"
	"github.com/gogf/gf/v2/os/gctx"
	"github.com/gogf/katyusha/krpc"
	"google.golang.org/grpc"

	"github.com/gogf/gf-tracing/examples/grpc_db_redis_log/protobuf/user"
	"github.com/gogf/gf-tracing/tracing"
)

const (
	ServiceName       = "tracing-grpc-client"
	JaegerUdpEndpoint = "localhost:6831"
)

func main() {
	ctx := gctx.New()
	tp, err := tracing.InitJaeger(ServiceName, JaegerUdpEndpoint)
	if err != nil {
		g.Log().Fatal(ctx, err)
	}

	defer tp.Shutdown(ctx)

	StartRequests()
}

// StartRequests start request
func StartRequests() {
	ctx, span := gtrace.NewSpan(context.Background(), "StartRequests")
	defer span.End()

	grpcClientOptions := make([]grpc.DialOption, 0)
	grpcClientOptions = append(
		grpcClientOptions,
		grpc.WithInsecure(),
		grpc.WithBlock(),
		grpc.WithChainUnaryInterceptor(
			krpc.Client.UnaryError,
			krpc.Client.UnaryTracing,
		),
	)

	conn, err := grpc.Dial(":8000", grpcClientOptions...)
	if err != nil {
		g.Log().Fatalf(ctx, "did not connect: %v", err)
	}
	defer conn.Close()

	client := user.NewUserClient(conn)

	// Baggage.
	ctx = gtrace.SetBaggageValue(ctx, "uid", 100)

	// Insert.
	insertRes, err := client.Insert(ctx, &user.InsertReq{
		Name: "john",
	})
	if err != nil {
		g.Log().Fatalf(ctx, `%+v`, err)
	}
	g.Log().Info(ctx, "insert:", insertRes.Id)

	// Query.
	queryRes, err := client.Query(ctx, &user.QueryReq{
		Id: insertRes.Id,
	})
	if err != nil {
		g.Log().Printf(ctx, `%+v`, err)
		return
	}
	g.Log().Info(ctx, "query:", queryRes)

	// Delete.
	_, err = client.Delete(ctx, &user.DeleteReq{
		Id: insertRes.Id,
	})
	if err != nil {
		g.Log().Printf(ctx, `%+v`, err)
		return
	}
	g.Log().Info(ctx, "delete:", insertRes.Id)

	// Delete with error.
	_, err = client.Delete(ctx, &user.DeleteReq{
		Id: -1,
	})
	if err != nil {
		g.Log().Printf(ctx, `%+v`, err)
		return
	}
	g.Log().Info(ctx, "delete:", -1)

}
