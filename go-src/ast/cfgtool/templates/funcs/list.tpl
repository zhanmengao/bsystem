
type TypeGet{{.BuildClassName}}CB func(conf []{{.Cfg.ValueType}})[]{{.Cfg.ValueType}}
type TypeFilter{{.BuildClassName}}CB func(conf {{.Cfg.ValueType}})bool

func load{{.BuildClassName}}(latestMsg proto.Unmarshaler) {
	msg,ok := latestMsg.(*pb.{{.Cfg.AryStructName}})
	if !ok{
	    xlog.LevelLogfn(xlog.FATAL, "load table {{ .Cfg.AryStructName }} proto error")
	    return
	}
	if len(msg.Ary) == 0 {
		return
	}
	tab := make([]{{.Cfg.ValueType}},len(msg.Ary))
	for i,v := range msg.Ary{
        tab[i] = v
	}
	Mock{{.BuildClassName}}(tab)
}

func Mock{{.BuildClassName}}(pbData []{{.Cfg.ValueType}}){
    {{.BuildObjName}}.isLoad = true
    {{.BuildObjName}}.metaData = newMetaData({{ .Cfg.SheetVar }}, pbData)
    tableChanged({{ .Cfg.SheetVar }})
}

//Get{{.BuildClassName}}Copy 返回一个拷贝的list
func Get{{.BuildClassName}}Copy(ctx interface{})[]{{.Cfg.ValueType}}{
	tab := Get{{.BuildClassName}}ReadOnly(ctx)
	if tab != nil {
		tab = depthCopy{{.BuildClassName}}List(tab)
	}
	return tab
}

func Get{{.BuildClassName}}ReadOnly(ctx interface{}) []{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		val := tab.([]{{.Cfg.ValueType}})
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           val = {{.BuildObjName}}.get{{.BuildClassName}}CB(val)
        }
        return val
	}
	return nil
}

//Get{{.BuildClassName}}Default 返回默认结构体（深拷贝）
func Get{{.BuildClassName}}Default() []{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getDefaultTable()
	if tab != nil {
		val := tab.([]{{.Cfg.ValueType}})
        var ret = val
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           ret = {{.BuildObjName}}.get{{.BuildClassName}}CB(ret)
        }
        return ret
	}
	return nil
}

func Get{{.BuildClassName}}ByFuncReadOnly(ctx interface{},cb TypeFilter{{.BuildClassName}}CB)[]{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		l := tab.([]{{.Cfg.ValueType}})
		var ret = make([]{{.Cfg.ValueType}},0,1)
		for _,v := range l{
		    if cb(v){
		        ret = append(ret,depthCopy{{.BuildClassName}}(v))
		    }
		}
		return ret
	}
	return nil
}

func Get{{.BuildClassName}}ByFuncCopy(ctx interface{},cb TypeFilter{{.BuildClassName}}CB)[]{{.Cfg.ValueType}} {
    rd := Get{{.BuildClassName}}ByFuncReadOnly(ctx,cb)
    ret := make([]{{.Cfg.ValueType}},len(rd))
    for i,v := range rd{
        ret[i] = depthCopy{{.BuildClassName}}(v)
    }
    return ret
}

func Get{{.BuildClassName}}ByFuncDefault(cb TypeFilter{{.BuildClassName}}CB)[]{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getDefaultTable()
	if tab != nil {
		l := tab.([]{{.Cfg.ValueType}})
		var ret = make([]{{.Cfg.ValueType}},0,1)
		for _,v := range l{
		    if cb(v){
		        ret = append(ret,v)
		    }
		}
		return ret
	}
	return nil
}



//depthCopy{{.BuildClassName}}List 深拷贝List
func depthCopy{{.BuildClassName}}List(arr []*pb.{{.Cfg.ConfigStructName}})[]*pb.{{.Cfg.ConfigStructName}}{
    cpArray := make([]*pb.{{.Cfg.ConfigStructName}},len(arr))
	for i,v := range arr{
	    cpArray[i] = depthCopy{{.BuildClassName}}(v)
	}
	return cpArray
}