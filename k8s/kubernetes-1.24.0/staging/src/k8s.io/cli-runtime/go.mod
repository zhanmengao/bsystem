// This is a generated file. Do not edit directly.

module k8s.io/cli-runtime

go 1.16

require (
	github.com/davecgh/go-spew v1.1.1
	github.com/evanphx/json-patch v4.12.0+incompatible
	github.com/google/gnostic v0.5.7-v3refs
	github.com/google/uuid v1.1.2
	github.com/liggitt/tabwriter v0.0.0-20181228230101-89fcab3d43de
	github.com/spf13/cobra v1.4.0
	github.com/spf13/pflag v1.0.5
	github.com/stretchr/testify v1.7.0
	golang.org/x/text v0.3.7
	gopkg.in/yaml.v2 v2.4.0
	k8s.io/api v0.0.0
	k8s.io/apimachinery v0.0.0
	k8s.io/client-go v0.0.0
	k8s.io/kube-openapi v0.0.0-20220328201542-3ee0da9b0b42
	k8s.io/utils v0.0.0-20220210201930-3a6ce19ff2f9
	sigs.k8s.io/kustomize/api v0.11.4
	sigs.k8s.io/kustomize/kyaml v0.13.6
	sigs.k8s.io/yaml v1.2.0
)

replace (
	k8s.io/api => ../api
	k8s.io/apimachinery => ../apimachinery
	k8s.io/cli-runtime => ../cli-runtime
	k8s.io/client-go => ../client-go
)
