{{ define "file" }}

//此文件为工具自动生成，不建议修改和提交上传gitlab

package pbconfig

import (
    "log"
    "fmt"
    "io/ioutil"
    "forevernine.com/ikpure/server/common/configmgr"
    "forevernine.com/ikpure/server/common/configmgr/abtest"
    "forevernine.com/ikpure/server/common/define/contextdefine"
    "forevernine.com/ikpure/server/common/define/protodefine"
    "forevernine.com/ikpure/server/common/global"

    "forevernine.com/ikpure/server/pbclass/pb"
)

const (
{{ range $key, $value := .SheetMapSheetName }}
{{ $key }} = "{{ $value.Sheet }}"
{{ end }}
)

var (
    _ = log.Ldate // 解决可能出现的包引入未使用的问题

    {{.BuildObjName}} *{{.BuildClassName}}
)

type {{.BuildClassName}} struct {
    isInitByLoad bool
    isLoad bool
    {{ range .SheetMapSheetName }}
        sheetCfg{{.Sheet}} *abtest.SheetCfg
    {{ end }}
}


// 加载配置, 构造单例模式; 必须在初始化流程调用; 推荐使用对象组合模式管理依赖
func Load{{.BuildClassName}}() (obj *{{.BuildClassName}}, err error) {
    if {{.BuildObjName}} == nil {
        {{.BuildObjName}} = &{{.BuildClassName}}{}
        {{.BuildObjName}}.isInitByLoad = true

        {{ range .SheetMapSheetName }}
            {{.BuildObjName}}.sheetCfg{{.Sheet}},err = abtest.NewSheetCfg({{.SheetVar}}, &pb.{{.AryStructName}}{}, {{.AbTest}})
            if err != nil {
                return
            }
            err = {{.BuildObjName}}.sheetCfg{{.Sheet}}.Reload()
            if err != nil {
                return
            }
        {{ end }}
    }

    obj = {{.BuildObjName}}
    if !obj.isInitByLoad {
    {{ range .SheetMapSheetName }}
    if _,sheetCfgObj := obj.sheetCfg{{.Sheet}}.GetDirect(nil); sheetCfgObj == nil {
        err = fmt.Errorf("sheetCfg{{.Sheet}} sheetCfgObj == nil, not load")
        return
    }
    {{ end }}
    }

    obj.isLoad = true
    return
}

// 获取单例; 必须先初始化; 函数式编程可选用这个函数做兼容替换
func Get{{.BuildClassName}}Ins() *{{.BuildClassName}} {
    if {{.BuildObjName}} == nil {
        // 正常业务都不会触发, 为了尽早在测试阶段发现配置未初始化, 并且提示更友好
        panic("{{.BuildObjName}} == nil")
    }

    if !{{.BuildObjName}}.isLoad { // mock测试中, 保证模块内部必须调用初始化, 不会出现mock无问题, srv进程起不来
        panic("{{.BuildObjName}} not isLoad")
    }
    return {{.BuildObjName}}
}

{{ range .LoadFuncs }}
{{ .FuncBody | unescaped }}
{{ end }}

// hook
var (
	{{.BuildObjName}}Hook = &{{.BuildClassName}}Hook{}
)
type {{.BuildClassName}}Hook struct {

}

func Get{{.BuildClassName}}Hook() *{{.BuildClassName}}Hook {
	return {{.BuildObjName}}Hook
}

{{ range $varName, $value := .SheetMapSheetName }}
   // HookSheet{{ $value.Sheet }} ; 注册配置变动回调函数
   func (c *{{.BuildClassName}}Hook) HookSheet{{ $value.Sheet }}(handle func() error) {
   	   configmgr.RegisterHook({{ $varName }}, handle)
   	   return
   }

   // TriggerHookSheet{{ $value.Sheet }} ; 触发配置变动回调函数
   func (c *{{.BuildClassName}}Hook) TriggerHookSheet{{ $value.Sheet }}() (err error) {
   	return configmgr.TriggerHook({{ $varName }})
   }
{{ end }}



// mock
var (
	{{.BuildObjName}}Mock = &{{.BuildClassName}}Mock{}
)
type {{.BuildClassName}}Mock struct {

}

func Get{{.BuildClassName}}Mock() *{{.BuildClassName}}Mock {
	if !global.IsInTest() {
		panic("mock must call in test")
	}

	// 初始化配置obj
	if {{.BuildObjName}} == nil {
        var err error
		{{.BuildObjName}} = &{{.BuildClassName}}{}
        {{ range .SheetMapSheetName }}
            {{.BuildObjName}}.sheetCfg{{.Sheet}},err = abtest.NewSheetCfg({{.SheetVar}}, &pb.{{.AryStructName}}{}, {{.AbTest}})
            if err != nil {
                panic(err)
            }
        {{ end }}
	}

	return {{.BuildObjName}}Mock
}

{{ range $varName, $value := .SheetMapSheetName }}
   // MockGet{{ $value.Sheet }}AryObj 获取已有的ary结构体
  func (m *{{.BuildClassName}}Mock) MockGet{{ $value.Sheet }}AryObj(ctx contextdefine.IContext) (ary *pb.{{.AryStructName}}) {
    // 深度拷贝
    ary = &pb.{{.AryStructName}}{}
    {{.BuildObjName}}.sheetCfg{{ $value.Sheet }}.GetObj(ctx, ary)
    return
  }

   // MockSheetSet{{ $value.Sheet }}AryObj ; 修改配置表ary结构体
   func (m *{{.BuildClassName}}Mock) MockSheetSet{{ $value.Sheet }}AryObj(ctx contextdefine.IContext, aryObj  protodefine.IProto) {
   	   target := abtest.GetSheetConfigTarget(ctx, {{ $varName }})
   	   {{.BuildObjName}}.sheetCfg{{ $value.Sheet }}.Set(target, aryObj)
   	   return
   }

   // MockSheetSet{{ $value.Sheet }}JsonBs 通过json修改配置表ary结构体
   func (m *{{.BuildClassName}}Mock) MockSheetSet{{ $value.Sheet }}JsonBs(ctx contextdefine.IContext, js []byte) {
   	target := abtest.GetSheetConfigTarget(ctx, {{ $varName }})
   	if err := {{.BuildObjName}}.sheetCfg{{ $value.Sheet }}.SetJsonBs(target, js); err != nil {
   		panic(err)
   	}
   	return
   }

   // MockSheetSet{{ $value.Sheet }}JsonFile 通过json file修改配置表ary结构体
   func (m *{{.BuildClassName}}Mock) MockSheetSet{{ $value.Sheet }}JsonFile(ctx contextdefine.IContext, testfile string) {
   	bs,err := ioutil.ReadFile(testfile)
   	if err != nil {
   		panic(err)
   	}

   	m.MockSheetSet{{ $value.Sheet }}JsonBs(ctx, bs)
   	return
   }

   // MockAdd{{ $value.Sheet }}Item 添加元素到ary结构体的列表
   func (m *{{.BuildClassName}}Mock) MockAdd{{ $value.Sheet }}Item(ctx contextdefine.IContext, item *pb.{{.ConfigStructName}}) {
   	// 深度拷贝
   	ary := m.MockGet{{ $value.Sheet }}AryObj(ctx)
   	ary.Ary = append(ary.Ary, item)
   	m.MockSheetSet{{ $value.Sheet }}AryObj(ctx, ary)
   	return
   }
{{ end }}





{{ end }}