
//TypeGet{{.BuildClassName}}CB
//	@Description:在调用表格GetByID产生结果后发生回调
//	@param ab：表格ABTest字段
//	@param conf：读出结果
//  @return ：修正后的结果
type TypeGet{{.BuildClassName}}CB func(ab string,conf {{.Cfg.ROPkgType}}){{.Cfg.ROPkgType}}


//Type{{.BuildClassName}}Changed
//	@Description:在表格Reload时发生回调
//	@param sheetName :表格Sheet名
//	@param filterName：表格ABTest字段
//	@param pbData :原始表格
//  @return ：修正后的表格
type Type{{.BuildClassName}}Changed func(sheetName,filterName string,pbData {{.Cfg.ValueType}}){{.Cfg.ValueType}}

type I{{.BuildClassName}} interface{
    Get{{.BuildClassName}}(ctx context.Context) {{.Cfg.ROPkgType}}
    Get{{.BuildClassName}}ABTest(ctx context.Context,ab string) {{.Cfg.ROPkgType}}
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
	Mock{{.BuildClassName}}(filterName,msg.Ary[0])
	return
}

func Mock{{.BuildClassName}}(filterName string,pbData {{.Cfg.ValueType}}){
    ctx := context.Background()
	for _, f := range on{{.BuildClassName}}Changed {
		if f != nil {
			safego.SafeFunc(ctx, func(ctx context.Context) {
				pbData = f({{ .Cfg.SheetVar }}, filterName, pbData)
			})
		}
	}
    is{{.BuildClassName}}Load = true
    meta := newMetaData({{ .Cfg.SheetVar }})
    meta = getOrLoadTable({{ .Cfg.SheetVar }},meta).(*tableMetaData)
    meta.register(filterName, pbData.GetReadOnly())
    meta{{.BuildClassName}}Data = meta
    xlog.Infof(context.Background(),"{{.BuildClassName}}#[%s].bytes changed .reload ",filterName)
}

func(p *{{.BuildStructName}})Get{{.BuildClassName}}(ctx context.Context) {{.Cfg.ROPkgType}} {
    return p.Get{{.BuildClassName}}ABTest(ctx,"")
}

func(p *{{.BuildStructName}})Get{{.BuildClassName}}ABTest(ctx context.Context,ab string) {{.Cfg.ROPkgType}} {
    if !is{{.BuildClassName}}Load {
        panic("{{.BuildClassName}} not isLoad")
    }
    var val {{.Cfg.ROPkgType}}
    span := ftrace.NewTableSpan(ctx,"{{.BuildClassName}}")
    defer func(){
        span.End("{{.CfgDefine}}",ab,"","",val)
    }()

	tab := meta{{.BuildClassName}}Data.getTable(ab)
	if tab != nil {
		val = tab.({{.Cfg.ROPkgType}})
        if p.get{{.BuildClassName}}CB != nil{
           val = p.get{{.BuildClassName}}CB(ab,val)
        }
        return val
	}
	return nil
}