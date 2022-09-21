
//TypeGet{{.BuildClassName}}CB
//	@Description:在调用表格GetByID产生结果后发生回调
//	@param ab：表格ABTest字段
//	@param conf：读出结果
//  @return ：修正后的结果
type TypeGet{{.BuildClassName}}CB func(ab string,conf []{{.Cfg.ROPkgType}})[]{{.Cfg.ROPkgType}}

//TypeFilter{{.BuildClassName}}CB
//	@Description:在表格Reload时发生回调
//	@param conf :表中字段
//	@bool 返回是否接受结果
type TypeFilter{{.BuildClassName}}CB func(conf {{.Cfg.ROPkgType}})bool

//Type{{.BuildClassName}}Changed
//	@Description:在表格Reload时发生回调
//	@param sheetName :表格Sheet名
//	@param filterName：表格ABTest字段
//	@param arr :原始表格
//  @return ：修正后的表格
type Type{{.BuildClassName}}Changed func(sheetName,filterName string,arr []{{.Cfg.ValueType}})[]{{.Cfg.ValueType}}

type I{{.BuildClassName}} interface{
    Register{{.BuildClassName}}CB(cb TypeGet{{.BuildClassName}}CB)
    Get{{.BuildClassName}}(ctx context.Context) []{{.Cfg.ROPkgType}}
    Get{{.BuildClassName}}ABTest(ctx context.Context,ab string) []{{.Cfg.ROPkgType}}
    Get{{.BuildClassName}}ByFunc(ctx context.Context,cb TypeFilter{{.BuildClassName}}CB,ab string)[]{{.Cfg.ROPkgType}}
}


func load{{.BuildClassName}}(filterName string,oldConfig interface{},newConfig interface{}) {
	msg,ok := newConfig.(*pb.{{.Cfg.AryStructName}})
	if !ok{
	    xlog.Fatalf(context.Background(), "load table {{ .Cfg.AryStructName }} proto error")
	    return
	}
	if len(msg.Ary) == 0 {
		return
	}
	tab := make([]{{.Cfg.ValueType}},len(msg.Ary))
	for i,v := range msg.Ary{
        tab[i] = v
	}
	Mock{{.BuildClassName}}(filterName,tab)
}

func Mock{{.BuildClassName}}(filterName string,arr []{{.Cfg.ValueType}}){
    ctx := context.Background()
    for _,f := range on{{.BuildClassName}}Changed{
        if f!=nil{
            safego.SafeFunc(ctx,func(ctx context.Context){
                arr = f({{ .Cfg.SheetVar }},filterName,arr)
            })
        }
    }
    is{{.BuildClassName}}Load = true
    var valList = make([]{{.Cfg.ROPkgType}},len(arr))
    for i,pbData := range arr{
        valList[i] = pb.Get{{.Cfg.ConfigStructName}}ReadOnly(pbData)
    }
    meta := newMetaData({{ .Cfg.SheetVar }})
    meta = getOrLoadTable({{ .Cfg.SheetVar }},meta).(*tableMetaData)
    meta.register(filterName, valList)
    meta{{.BuildClassName}}Data = meta
    xlog.Infof(context.Background(),"{{.BuildClassName}}#[%s].bytes changed .reload ",filterName)
}

func(p *{{.BuildStructName}}) Get{{.BuildClassName}}(ctx context.Context) []{{.Cfg.ROPkgType}} {
    return p.Get{{.BuildClassName}}ABTest(ctx,"")
}

func(p *{{.BuildStructName}}) Get{{.BuildClassName}}ABTest(ctx context.Context,ab string) []{{.Cfg.ROPkgType}} {
    if !is{{.BuildClassName}}Load {
        panic("{{.BuildClassName}} not isLoad")
    }
    var val []{{.Cfg.ROPkgType}}
    span := ftrace.NewTableSpan(ctx,"{{.BuildClassName}}")
    defer func() {
        span.End("{{.CfgDefine}}",ab,"","",p.getStringerList(val)...)
    }()

	tab := meta{{.BuildClassName}}Data.getTable(ab)
	if tab != nil {
		val = tab.([]{{.Cfg.ROPkgType}})
        if p.get{{.BuildClassName}}CB != nil{
           val = p.get{{.BuildClassName}}CB(ab,val)
        }
        return val
	}
	return nil
}


func(p *{{.BuildStructName}}) Get{{.BuildClassName}}ByFunc(ctx context.Context,cb TypeFilter{{.BuildClassName}}CB,ab string)[]{{.Cfg.ROPkgType}} {
    if !is{{.BuildClassName}}Load {
        panic("{{.BuildClassName}} not isLoad")
    }
    var ret []{{.Cfg.ROPkgType}}
    span := ftrace.NewTableSpan(ctx,"{{.BuildClassName}}")
    defer func(){
        span.End("{{.CfgDefine}}",ab,"","",p.getStringerList(ret)...)
    }()

	tab := meta{{.BuildClassName}}Data.getTable(ab)
	if tab != nil {
		l := tab.([]{{.Cfg.ROPkgType}})
		ret = make([]{{.Cfg.ROPkgType}},0,1)
		for _,v := range l{
		    if cb(v){
		        ret = append(ret,v)
		    }
		}
		return ret
	}
	return nil
}
