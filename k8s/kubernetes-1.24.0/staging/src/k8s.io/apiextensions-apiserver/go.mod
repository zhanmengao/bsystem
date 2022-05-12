// This is a generated file. Do not edit directly.

module k8s.io/apiextensions-apiserver

go 1.16

require (
	github.com/emicklei/go-restful v2.9.5+incompatible
	github.com/gogo/protobuf v1.3.2
	github.com/google/cel-go v0.10.1
	github.com/google/gnostic v0.5.7-v3refs
	github.com/google/go-cmp v0.5.5
	github.com/google/gofuzz v1.1.0
	github.com/google/uuid v1.1.2
	github.com/mitchellh/mapstructure v1.4.1 // indirect
	github.com/spf13/cobra v1.4.0
	github.com/spf13/pflag v1.0.5
	github.com/stretchr/testify v1.7.0
	go.etcd.io/etcd/client/pkg/v3 v3.5.1
	go.etcd.io/etcd/client/v3 v3.5.1
	google.golang.org/genproto v0.0.0-20220107163113-42d7afdf6368
	google.golang.org/grpc v1.40.0
	google.golang.org/protobuf v1.27.1
	gopkg.in/yaml.v2 v2.4.0
	k8s.io/api v0.0.0
	k8s.io/apimachinery v0.0.0
	k8s.io/apiserver v0.0.0
	k8s.io/client-go v0.0.0
	k8s.io/code-generator v0.0.0
	k8s.io/component-base v0.0.0
	k8s.io/klog/v2 v2.60.1
	k8s.io/kube-openapi v0.0.0-20220328201542-3ee0da9b0b42
	k8s.io/utils v0.0.0-20220210201930-3a6ce19ff2f9
	sigs.k8s.io/json v0.0.0-20211208200746-9f7c6b3444d2
	sigs.k8s.io/structured-merge-diff/v4 v4.2.1
	sigs.k8s.io/yaml v1.2.0
)

replace (
	k8s.io/api => ../api
	k8s.io/apiextensions-apiserver => ../apiextensions-apiserver
	k8s.io/apimachinery => ../apimachinery
	k8s.io/apiserver => ../apiserver
	k8s.io/client-go => ../client-go
	k8s.io/code-generator => ../code-generator
	k8s.io/component-base => ../component-base
)
