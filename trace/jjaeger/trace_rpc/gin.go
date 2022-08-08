package trace_rpc

import (
	"github.com/gin-gonic/gin"
	"go.opentelemetry.io/contrib/instrumentation/github.com/gin-gonic/gin/otelgin"
)

func TracePlug(srvName string) gin.HandlerFunc {
	return otelgin.Middleware(srvName)
}
