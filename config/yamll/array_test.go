package yamll

import (
	"fmt"
	"io/ioutil"
	"log"
	"testing"

	yaml "gopkg.in/yaml.v3"
)

func TestArray(t *testing.T) {
	cluster := new(YamlArr)
	yamlFile, err := ioutil.ReadFile("cluster.yaml")
	if err != nil {
		log.Println(err)
	}

	err = yaml.Unmarshal(yamlFile, &cluster)

	if err != nil {
		log.Println(err)
	}

	log.Println("conf", cluster)
	fmt.Println(cluster.Testclu.Hostips)

}
