{{ define "file" }}

//此文件为工具自动生成，不建议修改和提交上传gitlab

package forevernine_com_config

import (
	pb "{{ .ProtoPackage }}"
	"github.com/gogo/protobuf/proto"
	baseConfig "forevernine.com/base/srvcore/config"
	"forevernine.com/base/srvcore/libs/xlog"

	"io/ioutil"
)

const (
{{ .Cfg.SheetVar }} = "{{ .Cfg.Sheet }}"
)



func init(){
     registerWatchCB({{ .Cfg.SheetVar }},watch{{.BuildClassName}}Table)
}


var (
    {{.BuildObjName}}  {{.BuildClassName}}
)


type {{.BuildClassName}} struct {
    isLoad bool
    get{{.BuildClassName}}CB    TypeGet{{.BuildClassName}}CB
    metaData tableMetaInterface
}


//Register{{.BuildClassName}}CB 注册GetByID调用后的回调函数，可用于修正读表返回值
func Register{{.BuildClassName}}CB(cb TypeGet{{.BuildClassName}}CB){
    {{.BuildObjName}}.get{{.BuildClassName}}CB = cb
}

func Mock{{.BuildClassName}}FromBytes(filePath string)error{
    bt, err := ioutil.ReadFile(filePath)
    if err!=nil{
        return err
    }
    var config = new(pb.{{.Cfg.AryStructName}})
    err = config.Unmarshal(bt)
    if err!=nil{
        return err
    }
    load{{.BuildClassName}}(config)
    return nil
}

//depthCopy{{.BuildClassName}} 深拷贝
func depthCopy{{.BuildClassName}}(conf {{.Cfg.ValueType}}){{.Cfg.ValueType}}{
	data,_ := conf.Marshal()
	var ret = new(pb.{{.Cfg.ConfigStructName}})
    ret.Unmarshal(data)
    return ret
}

{{ range .LoadFuncs }}
{{ .FuncBody | unescaped }}
{{ end }}

func watch{{.BuildClassName}}Table(){
    if err := baseConfig.WatchTableUpdate({{ .Cfg.SheetVar }}, load{{.BuildClassName}}); err != nil {
        panic(err)
    }
    xlog.LevelLogfn(xlog.INFO, "watch table %s ", {{ .Cfg.SheetVar }})
}








{{ end }}