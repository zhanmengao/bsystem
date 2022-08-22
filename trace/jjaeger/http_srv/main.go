package main

import "github.com/gin-gonic/gin"

func main() {
	r := gin.Default()
	// 插入中间件处理
	r.Use(UseOpenTracing())
	r.GET("/Get", GetUserInfo)
	panic(r.Run("0.0.0.0:8081")) // listen and serve on 0.0.0.0:8080 (for windows "localhost:8080")
}
