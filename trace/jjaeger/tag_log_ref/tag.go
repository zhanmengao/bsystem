package tag_log_ref

import (
	"github.com/opentracing/opentracing-go"
	"github.com/opentracing/opentracing-go/ext"
)

/*
Jaeger 的链路追踪中，可以携带 Tag 和 Log，他们都是键值对的形式：
                        {
                            "key": "http.method",
                            "type": "string",
                            "value": "GET"
                        },
*/
func setTag(sp opentracing.Span, url string) {
	//因为 opentracing 已经规定了所有的 Tag 类型，所以我们只需要调用 ext.xxx.Set() 设置即可。
	ext.HTTPUrl.Set(sp, url)
}
