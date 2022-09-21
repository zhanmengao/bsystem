package cfgtool

import (
	"config/f9cfg/internal/generator"
	"config/f9cfg/internal/pbpkg"
	"config/f9cfg/internal/ro"
	"github.com/spf13/cobra"
	"path"
)

type GenConfig struct {
	protoDir  string
	pbPackage string
	outputDir string
	pbGoDir   string
}

var r GenConfig

var ConfigTool = &cobra.Command{
	Use:   r.Name(),
	Short: r.Synopsis(),
	Long:  r.Synopsis(),
	Run:   r.Execute,
}

func init() {
	ConfigTool.Flags().StringVar(&r.protoDir, "d", ".", "proto仓库目录")
	ConfigTool.Flags().StringVar(&r.pbPackage, "p", "", "pb的package name")
	ConfigTool.Flags().StringVar(&r.outputDir, "o", "go/gconf/", "cfg.go输出目录")
	ConfigTool.Flags().StringVar(&r.pbGoDir, "go", "", ".pb.go目录")
}

func (r *GenConfig) Execute(cmd *cobra.Command, args []string) {
	if r.pbGoDir == "" {
		r.pbGoDir = path.Join(r.protoDir, "go", "pb")
	}
	if r.pbPackage == "" {
		tableProto := path.Join(r.protoDir, "proto", "table.proto")
		pbPackage, err := pbpkg.GetPBPackage(tableProto)
		if err != nil {
			panic(err)
		}
		r.pbPackage = pbPackage
	}
	pkgName := ro.Run(r.pbGoDir)
	generator.Run(r.pbGoDir, r.pbPackage, pkgName, r.outputDir)
}

func (r *GenConfig) Name() string {
	return "cfg"
}

func (r *GenConfig) Synopsis() string {
	return "生成配置文件pb对象的RO包装，生成读表代码"
}

func (r *GenConfig) Usage() string {
	return "生成配置文件pb对象的RO包装，生成读表代码"
}
