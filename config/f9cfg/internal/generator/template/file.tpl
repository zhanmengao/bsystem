//此文件为工具自动生成，不要编辑哦

package gconf

import (
	pb "{{ .ProtoPackage }}"
	"forevernine.com/midplat/base_libs/safego"
	"forevernine.com/midplat/base_server/config/tabcfg"
	"forevernine.com/midplat/base_server/ftrace"
	"forevernine.com/midplat/base_libs/xlog"
    "context"
	"io/ioutil"
	"fmt"
)

//{{.CfgDefine}}

const (
    {{ .Cfg.SheetVar }} = "{{ .Cfg.Sheet }}"
)



func init(){
     registerWatchCB({{ .Cfg.SheetVar }},watch{{.BuildClassName}}Table)
}


var (
    is{{.BuildClassName}}Load bool
    meta{{.BuildClassName}}Data iMeta
    on{{.BuildClassName}}Changed  []Type{{.BuildClassName}}Changed
)

type {{.BuildStructName}} struct{
    get{{.BuildClassName}}CB    TypeGet{{.BuildClassName}}CB
}

//New{{.BuildClassName}} 获取表格变量，并将.bytes文件加载。须在加载配置中心后调用
func New{{.BuildClassName}}()I{{.BuildClassName}}{
	RequestTable({{ .Cfg.SheetVar }})
    return &{{.BuildStructName}}{}
}

{{ range .LoadFuncs }}
{{ .FuncBody | unescaped}}
{{ end }}


//Register{{.BuildClassName}}CB 注册GetByID调用后的回调函数，可用于修正读表返回值
func(p *{{.BuildStructName}})Register{{.BuildClassName}}CB(cb TypeGet{{.BuildClassName}}CB){
    p.get{{.BuildClassName}}CB = cb
}

func(p *{{.BuildStructName}})getStringerList(ary []{{.Cfg.ROPkgType}}) []fmt.Stringer{
	ret := make([]fmt.Stringer, 0, len(ary))
	for _, v := range ret {
		ret = append(ret, v)
	}
	return ret
}

func Register{{.BuildClassName}}Changed(cb Type{{.BuildClassName}}Changed){
    on{{.BuildClassName}}Changed = append(on{{.BuildClassName}}Changed,cb)
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
    _, filter := tabcfg.GetSheetFilter(filePath)
    load{{.BuildClassName}}(filter,nil,config)
    return nil
}


func watch{{.BuildClassName}}Table(){
    if err := tabcfg.WatchTableUpdate({{ .Cfg.SheetVar }}, load{{.BuildClassName}}); err != nil {
        panic(err)
    }
    xlog.Infof(context.Background(), "watch table %s ", {{ .Cfg.SheetVar }})
}