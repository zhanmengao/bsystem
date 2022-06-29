package main

import (
	"context"
	"fmt"
	"net"
	"time"

	"github.com/gogf/gf/v2/database/gdb"
	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/os/gcache"
	"github.com/gogf/gf/v2/os/gctx"
	"github.com/gogf/katyusha/krpc"
	"google.golang.org/grpc"

	"github.com/gogf/gf-tracing/examples/grpc_db_redis_log/protobuf/user"
	"github.com/gogf/gf-tracing/tracing"
)

type server struct{}

const (
	// ServiceName .
	ServiceName = "tracing-grpc-server"
	// JaegerUDPEndpoint .
	JaegerUDPEndpoint = "localhost:6831"
)

func main() {
	ctx := gctx.New()
	tp, err := tracing.InitJaeger(ServiceName, JaegerUDPEndpoint)
	if err != nil {
		g.Log().Fatal(ctx, err)
	}
	// Cleanly shutdown and flush telemetry when the application exits.
	defer tp.Shutdown(ctx)

	g.DB().GetCache().SetAdapter(gcache.NewAdapterRedis(g.Redis()))

	address := ":8000"
	listen, err := net.Listen("tcp", address)
	if err != nil {
		g.Log().Fatalf(ctx, "failed to listen: %v", err)
	}
	s := grpc.NewServer(
		grpc.ChainUnaryInterceptor(
			krpc.Server.UnaryError,
			krpc.Server.UnaryRecover,
			krpc.Server.UnaryTracing,
			krpc.Server.UnaryValidate,
		),
	)
	user.RegisterUserServer(s, &server{})
	g.Log().Printf(ctx, "grpc server starts listening on %s", address)
	if err := s.Serve(listen); err != nil {
		g.Log().Fatalf(ctx, "failed to serve: %v", err)
	}
}

// Insert is a route handler for inserting user info into dtabase.
func (s *server) Insert(ctx context.Context, req *user.InsertReq) (*user.InsertRes, error) {
	res := user.InsertRes{}
	result, err := g.Model("user").Ctx(ctx).Insert(g.Map{
		"name": req.Name,
	})
	if err != nil {
		return nil, err
	}
	id, _ := result.LastInsertId()
	res.Id = int32(id)
	return &res, nil
}

// Query is a route handler for querying user info. It firstly retrieves the info from redis,
// if there's nothing in the redis, it then does db select.
func (s *server) Query(ctx context.Context, req *user.QueryReq) (*user.QueryRes, error) {
	res := user.QueryRes{}
	err := g.Model("user").
		Ctx(ctx).
		Cache(gdb.CacheOption{Duration: 5 * time.Second, Name: s.userCacheKey(req.Id)}).
		WherePri(req.Id).
		Scan(&res)
	if err != nil {
		return nil, err
	}
	return &res, nil
}

// Delete is a route handler for deleting specified user info.
func (s *server) Delete(ctx context.Context, req *user.DeleteReq) (*user.DeleteRes, error) {
	res := user.DeleteRes{}
	_, err := g.Model("user").
		Ctx(ctx).
		Cache(gdb.CacheOption{Duration: -1 * time.Second, Name: s.userCacheKey(req.Id)}).
		WherePri(req.Id).
		Delete()
	if err != nil {
		return nil, err
	}
	return &res, nil
}

func (s *server) userCacheKey(id int32) string {
	return fmt.Sprintf(`userInfo:%d`, id)
}
