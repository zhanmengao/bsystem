package yamll

import (
	"fmt"
	"gopkg.in/yaml.v2"
	"io/ioutil"
	"testing"
)

func TestMap(t *testing.T) {
	body, err := ioutil.ReadFile("midplat.yaml")
	if err != nil {
		t.Fatal(err)
	}
	kv := make(map[interface{}]interface{})
	if err = yaml.Unmarshal(body, &kv); err != nil {
		t.Fatal(err)
	}
	for k, v := range kv {
		fmt.Printf("%s -> %d\n", k, v)
	}
}
