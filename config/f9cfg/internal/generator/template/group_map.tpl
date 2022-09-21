
//TypeGet{{.BuildClassName}}CB
//	@Description:在调用表格GetByID产生结果后发生回调
//	@param ab：表格ABTest字段
//	@param conf：读出结果
//  @return ：修正后的结果
type TypeGet{{.BuildClassName}}CB func(ab string,conf {{.Cfg.ROPkgType}}){{.Cfg.ROPkgType}}

//TypeFilter{{.BuildClassName}}CB
//	@Description:在表格Reload时发生回调
//	@param conf :表中字段
//	@bool 返回是否接受结果
type TypeFilter{{.BuildClassName}}CB func(conf {{.Cfg.ROPkgType}})bool

//Type{{.BuildClassName}}Changed
//	@Description:在表格Reload时发生回调
//	@param sheetName :表格Sheet名
//	@param filterName：表格ABTest字段
//	@param m :原始表格
//  @return ：修正后的表格
type Type{{.BuildClassName}}Changed func(sheetName,filterName string,m Type{{.BuildClassName}}Main)Type{{.BuildClassName}}Main

//Type{{.BuildClassName}}Main group map的一级map原始类型
type Type{{.BuildClassName}}Main map[{{.Cfg.KeyType}}]Type{{.BuildClassName}}Sub

//Type{{.BuildClassName}}Sub group map的sub map原始类型
type Type{{.BuildClassName}}Sub map[{{.Cfg.SubKeyType}}]{{.Cfg.ValueType}}

//Type{{.BuildClassName}}MainRO group map的一级map RO类型
type Type{{.BuildClassName}}MainRO map[{{.Cfg.KeyType}}]Type{{.BuildClassName}}SubRO

//Type{{.BuildClassName}}SubRO group map的sub map RO类型
type Type{{.BuildClassName}}SubRO map[{{.Cfg.SubKeyType}}]{{.Cfg.ROPkgType}}

type I{{.BuildClassName}} interface{
    Register{{.BuildClassName}}CB(cb TypeGet{{.BuildClassName}}CB)
    Get{{.BuildClassName}}ByID(ctx context.Context,{{.Cfg.Key}} {{.Cfg.KeyType}},{{.Cfg.SubKey}} {{.Cfg.SubKeyType}}) {{.Cfg.ROPkgType}}
    Get{{.BuildClassName}}ByIDABTest(ctx context.Context,{{.Cfg.Key}} {{.Cfg.KeyType}},{{.Cfg.SubKey}} {{.Cfg.SubKeyType}},ab string) {{.Cfg.ROPkgType}}
    Get{{.BuildClassName}}ByFunc(ctx context.Context,cb TypeFilter{{.BuildClassName}}CB,ab string) []{{.Cfg.ROPkgType}}
    Get{{.BuildClassName}}(ab string)Type{{.BuildClassName}}MainRO
}

func load{{.BuildClassName}}(filterName string,oldConfig interface{},newConfig interface{}) {
	msg,ok := newConfig.(*pb.{{.Cfg.AryStructName}})
	if !ok{
        xlog.Fatalf(context.Background(), "load table {{.Cfg.AryStructName}} proto error")
	    return
	}
	if len(msg.Ary) == 0 {
		return
	}
    m := make(Type{{.BuildClassName}}Main, len(msg.Ary)/2)
    for _,item := range msg.Ary {
        if val,ok := m[item.{{.Cfg.Key}}];!ok{
            val = make(Type{{.BuildClassName}}Sub,1)
            m[item.{{.Cfg.Key}}] = val
        }
        if _,exist := m[item.{{.Cfg.Key}}][item.{{.Cfg.SubKey}}];exist{
            xlog.Fatalf(context.Background(), "set table .Cfg.AryStructName repeated. item =  %s ",item.String())
        }else{
            m[item.{{.Cfg.Key}}][item.{{.Cfg.SubKey}}]  = item
        }
    }
    Mock{{.BuildClassName}}(filterName,m)
	return
}

func Mock{{.BuildClassName}}(filterName string,m Type{{.BuildClassName}}Main){
    ctx := context.Background()
    for _,f := range on{{.BuildClassName}}Changed{
        if f!=nil{
            safego.SafeFunc(ctx,func(ctx context.Context){
                m = f({{ .Cfg.SheetVar }},filterName,m)
            })
        }
    }
    is{{.BuildClassName}}Load = true
    var confMap = make(Type{{.BuildClassName}}MainRO)
    for k,arr := range m {
        val := make(Type{{.BuildClassName}}SubRO,len(arr))
        for i,pbData := range arr{
            val[i] = pb.Get{{.Cfg.ConfigStructName}}ReadOnly(pbData)
        }
        confMap[k] = val
    }
    meta := newMetaData({{ .Cfg.SheetVar }})
    meta = getOrLoadTable({{ .Cfg.SheetVar }},meta).(*tableMetaData)
    meta.register(filterName, confMap)
    meta{{.BuildClassName}}Data = meta
    xlog.Infof(context.Background(),"{{.BuildClassName}}#[%s].bytes changed .reload ",filterName)
}

func(p *{{.BuildStructName}}) Get{{.BuildClassName}}ByID(ctx context.Context,{{.Cfg.Key}} {{.Cfg.KeyType}},{{.Cfg.SubKey}} {{.Cfg.SubKeyType}}) {{.Cfg.ROPkgType}} {
    return p.Get{{.BuildClassName}}ByIDABTest(ctx,{{.Cfg.Key}},{{.Cfg.SubKey}},"")
}

func(p *{{.BuildStructName}}) Get{{.BuildClassName}}ByIDABTest(ctx context.Context,{{.Cfg.Key}} {{.Cfg.KeyType}},{{.Cfg.SubKey}} {{.Cfg.SubKeyType}},ab string) {{.Cfg.ROPkgType}} {
    if !is{{.BuildClassName}}Load {
        panic("{{.BuildClassName}} not isLoad")
    }
    var ret {{.Cfg.ROPkgType}}
    span := ftrace.NewTableSpan(ctx,"{{.BuildClassName}}")
    defer func(){
        span.End("{{.CfgDefine}}",ab,{{.Cfg.Key}},{{.Cfg.SubKey}},ret)
    }()

	tab := meta{{.BuildClassName}}Data.getTable(ab)
	if tab != nil {
		m := tab.(Type{{.BuildClassName}}MainRO)

		if subMap,ok := m[{{.Cfg.Key}}];ok{
			if ret,ok = subMap[{{.Cfg.SubKey}}];ok{
    		}
		}
        if p.get{{.BuildClassName}}CB != nil{
           ret = p.get{{.BuildClassName}}CB(ab,ret)
        }
		return ret
	}
	return nil
}


//Get{{.BuildClassName}}ByFunc 根据回调函数筛选对象，返回筛选后的切片
func(p *{{.BuildStructName}}) Get{{.BuildClassName}}ByFunc(ctx context.Context,cb TypeFilter{{.BuildClassName}}CB,ab string) []{{.Cfg.ROPkgType}} {
    if !is{{.BuildClassName}}Load {
        panic("{{.BuildClassName}} not isLoad")
    }
    var ret = make([]{{.Cfg.ROPkgType}},0,1)
    span := ftrace.NewTableSpan(ctx,"{{.BuildClassName}}")
    defer func(){
        span.End("{{.CfgDefine}}",ab,"","",p.getStringerList(ret)...)
    }()
	tab := meta{{.BuildClassName}}Data.getTable(ab)
	if tab != nil {
		m := tab.(Type{{.BuildClassName}}MainRO)
		for _,l := range m{
		    for _,v := range l{
		        if cb(v){
		            ret = append(ret, v)
		        }
		    }
		}
		return ret
	}
	return nil
}

//Get{{.BuildClassName}} 返回一个拷贝的map
func(p *{{.BuildStructName}}) Get{{.BuildClassName}}(ab string)Type{{.BuildClassName}}MainRO{
    if !is{{.BuildClassName}}Load {
        panic("{{.BuildClassName}} not isLoad")
    }
	tab := meta{{.BuildClassName}}Data.getTable(ab)
	if tab != nil {
		m := tab.(Type{{.BuildClassName}}MainRO)
		cpMap := make(Type{{.BuildClassName}}MainRO,len(m))
		for k,v := range m{
		    ms := make(Type{{.BuildClassName}}SubRO,len(v))
		    for ks,vs := range v{
		        ms[ks] = vs
		    }
		    cpMap[k] = ms
		}
		return cpMap
	}
	return nil
}
