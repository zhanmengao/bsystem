package viperr

import (
	"fmt"
	"github.com/spf13/viper"
	"os"
	"testing"
)

func TestDemo(t *testing.T) {
	//获取项目的执行路径
	path, err := os.Getwd()
	if err != nil {
		panic(err)
	}

	config := viper.New()

	config.AddConfigPath(path)   //设置读取的文件路径
	config.SetConfigName("demo") //设置读取的文件名
	config.SetConfigType("yaml") //设置文件的类型
	//尝试进行配置读取
	if err := config.ReadInConfig(); err != nil {
		panic(err)
	}

	//打印文件读取出来的内容:
	fmt.Println(config.Get("database.host"))
	fmt.Println(config.Get("database.user"))
	fmt.Println(config.Get("database.dbname"))
	fmt.Println(config.Get("database.pwd"))

}
