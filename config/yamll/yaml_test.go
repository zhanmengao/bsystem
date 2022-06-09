package yamll

import (
	"gopkg.in/yaml.v3"
	"io/ioutil"
	"testing"
)

func TestYaml(t *testing.T) {
	body, err := ioutil.ReadFile("midplat.yaml")
	if err != nil {
		t.Fatal(err)
	}
	cfg := &MidplatConfig{}
	if err = yaml.Unmarshal(body, cfg); err != nil {
		t.Fatal(err)
	}
	t.Log(cfg)
}
