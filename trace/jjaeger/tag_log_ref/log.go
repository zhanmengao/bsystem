package tag_log_ref

import (
	"github.com/opentracing/opentracing-go"
	"github.com/opentracing/opentracing-go/log"
)

//通过 span 对象调用函数即可设置日志
//示例(在中间件里面加一下)：
func setLog(sp opentracing.Span) {
	sp.LogFields(
		log.String("event", "soft error"),
		log.String("type", "cache timeout"),
		log.Int("waited.millis", 1500))
}
