package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"net/http"
)

func GetUserInfo(ctx *gin.Context) {
	userName := ctx.Param("username")
	fmt.Println("收到请求，用户名称为:", userName)
	ctx.String(http.StatusOK, "他的博客是 https://whuanle.cn")
}
