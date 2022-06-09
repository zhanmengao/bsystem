package viperr

import (
	"fmt"
	"github.com/spf13/viper"
	"testing"
)

var Conf *Config

func TestSQL(t *testing.T) {
	var err error

	v := viper.New()
	v.SetConfigName("mysql")
	v.SetConfigType("yaml")
	v.AddConfigPath("xxxxxx")

	err = v.ReadInConfig()
	if err != nil {
		fmt.Printf("[viper] ReadInConfig err:%s", err.Error())
		return
	}

	err = v.Unmarshal(&Conf)
	if err != nil {
		fmt.Printf("[viper] Unmarshal err:%s", err.Error())
		panic(err)
	}

	// 省略...
	return
}
