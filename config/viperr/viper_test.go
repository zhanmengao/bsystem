package viperr

import (
	"config/yamll"
	"fmt"
	"testing"
)
import "github.com/spf13/viper"

func TestYaml(t *testing.T) {
	vip := viper.New()
	vip.SetConfigFile("midplat.yaml")
	vip.SetConfigType("yaml")

	if err := vip.ReadInConfig(); err != nil {
		t.Fatal(err)
	}
	cfg := &yamll.BasicConfig{}
	err := vip.UnmarshalKey("basecfg", cfg)
	if err != nil {
		t.Fatal(err)
	}
	fmt.Println(cfg)
}

func TestYamlAll(t *testing.T) {
	vip := viper.New()
	vip.SetConfigFile("midplat.yaml")
	vip.SetConfigType("yaml")

	if err := vip.ReadInConfig(); err != nil {
		t.Fatal(err)
	}
	cfg := &yamll.MidplatConfig{}
	err := vip.Unmarshal(cfg)
	if err != nil {
		t.Fatal(err)
	}
	fmt.Println(cfg)
}

func TestKey(t *testing.T) {
	vip := viper.New()
	vip.SetConfigFile("midplat.yaml")
	vip.SetConfigType("yaml")

	if err := vip.ReadInConfig(); err != nil {
		t.Fatal(err)
	}
	fmt.Println(vip.Get("basecfg.platform"))
	fmt.Println(vip.Get("basecfg.sess_expire"))
	fmt.Println(vip.Get("basecfg.coordinator_addr"))
}
