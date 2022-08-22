package main

import (
	"context"
	"github.com/gogf/gf/v2/frame/g"
	"github.com/gogf/gf/v2/net/gtrace"
	"github.com/gogf/gf/v2/util/gutil"
)

//方法间Span创建

// GetUser retrieves and returns hard coded user data for demonstration.
func GetUser(ctx context.Context, id int) g.Map {
	ctx, span := gtrace.NewSpan(ctx, "GetUser")
	defer span.End()
	m := g.Map{}
	gutil.MapMerge(
		m,
		GetInfo(ctx, id),
		GetDetail(ctx, id),
		GetScores(ctx, id),
	)
	return m
}

// GetInfo retrieves and returns hard coded user info for demonstration.
func GetInfo(ctx context.Context, id int) g.Map {
	ctx, span := gtrace.NewSpan(ctx, "GetInfo")
	defer span.End()
	if id == 100 {
		return g.Map{
			"id":     100,
			"name":   "john",
			"gender": 1,
		}
	}
	return nil
}

// GetDetail retrieves and returns hard coded user detail for demonstration.
func GetDetail(ctx context.Context, id int) g.Map {
	ctx, span := gtrace.NewSpan(ctx, "GetDetail")
	defer span.End()
	if id == 100 {
		return g.Map{
			"site":  "https://goframe.org",
			"email": "john@goframe.org",
		}
	}
	return nil
}

// GetScores retrieves and returns hard coded user scores for demonstration.
func GetScores(ctx context.Context, id int) g.Map {
	ctx, span := gtrace.NewSpan(ctx, "GetScores")
	defer span.End()
	if id == 100 {
		return g.Map{
			"math":    100,
			"english": 60,
			"chinese": 50,
		}
	}
	return nil
}

//该示例代码展示了多层级方法间的链路信息传递，即是把ctx上下文变量作为第一个方法参数传递即可。在方法内部，我们通过的固定语法来创建/开始一个Span
/*
ctx, span := gtrace.NewSpan(ctx, "xxx")
defer span.End()
*/

//并通过defer的方式调用span.End来结束一个Span，这样可以很好地记录Span生命周期（开始和结束）信息，这些信息都将会展示到链路跟踪系统中。
//其中gtrace.NewSpan方法的第二个参数spanName我们直接给定方法的名称即可，这样在链路展示中比较有识别性。
