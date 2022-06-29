package main

import (
	"fmt"
	"time"

	"github.com/gogf/gf/v2/database/gdb"
	"github.com/gogf/gf/v2/errors/gerror"
	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/ghttp"
	"github.com/gogf/gf/v2/os/gcache"
	"github.com/gogf/gf/v2/os/gctx"

	"github.com/gogf/gf-tracing/tracing"
)

type tracingApi struct{}

const (
	ServiceName       = "tracing-http-server"
	JaegerUdpEndpoint = "localhost:6831"
)

func main() {
	ctx := gctx.New()
	tp, err := tracing.InitJaeger(ServiceName, JaegerUdpEndpoint)
	if err != nil {
		g.Log().Fatal(ctx, err)
	}
	// Cleanly shutdown and flush telemetry when the application exits.
	defer tp.Shutdown(ctx)
	g.DB().GetCache().SetAdapter(gcache.NewAdapterRedis(g.Redis()))
	s := g.Server()
	s.Group("/", func(group *ghttp.RouterGroup) {
		group.ALL("/user", new(tracingApi))
	})
	s.SetPort(8199)
	s.Run()
}

type userApiInsert struct {
	Name string `v:"required#Please input user name."`
}

// Insert is a route handler for inserting user info into dtabase.
func (api *tracingApi) Insert(r *ghttp.Request) {
	var (
		dataReq *userApiInsert
	)
	if err := r.Parse(&dataReq); err != nil {
		r.Response.WriteExit(gerror.Current(err))
	}
	result, err := g.Model("user").Ctx(r.Context()).Insert(g.Map{
		"name": dataReq.Name,
	})
	if err != nil {
		r.Response.WriteExit(gerror.Current(err))
	}
	id, _ := result.LastInsertId()
	r.Response.Write(id)
}

type userApiQuery struct {
	Id int `v:"min:1#User id is required for querying."`
}

// Query is a route handler for querying user info. It firstly retrieves the info from redis,
// if there's nothing in the redis, it then does db select.
func (api *tracingApi) Query(r *ghttp.Request) {
	var (
		dataReq *userApiQuery
	)
	if err := r.Parse(&dataReq); err != nil {
		r.Response.WriteExit(gerror.Current(err))
	}
	one, err := g.Model("user").Ctx(r.GetCtx()).Cache(gdb.CacheOption{Duration: 5 * time.Second, Name: api.userCacheKey(dataReq.Id)}).One(dataReq.Id)
	if err != nil {
		r.Response.WriteExit(gerror.Current(err))
	}
	r.Response.WriteJson(one)
}

type userApiDelete struct {
	Id int `v:"min:1#User id is required for deleting."`
}

// Delete is a route handler for deleting specified user info.
func (api *tracingApi) Delete(r *ghttp.Request) {
	var (
		dataReq *userApiDelete
	)
	if err := r.Parse(&dataReq); err != nil {
		r.Response.WriteExit(gerror.Current(err))
	}
	_, err := g.Model("user").
		Ctx(r.Context()).
		Cache(gdb.CacheOption{Duration: -1 * time.Second, Name: api.userCacheKey(dataReq.Id)}).
		WherePri(dataReq.Id).
		Delete()
	if err != nil {
		r.Response.WriteExit(gerror.Current(err))
	}
	r.Response.Write("ok")
}

func (api *tracingApi) userCacheKey(id int) string {
	return fmt.Sprintf(`userInfo:%d`, id)
}
