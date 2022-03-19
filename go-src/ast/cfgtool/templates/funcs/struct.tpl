
type TypeGet{{.BuildClassName}}CB func(conf {{.Cfg.ValueType}}){{.Cfg.ValueType}}

func load{{.BuildClassName}}(latestMsg proto.Unmarshaler) {
	msg,ok := latestMsg.(*pb.{{.Cfg.AryStructName}})
	if !ok{
	    xlog.LevelLogfn(xlog.FATAL, "load table {{ .Cfg.AryStructName }} proto error")
	    return
	}
	if len(msg.Ary) == 0 {
		return
	}
	Mock{{.BuildClassName}}(msg.Ary[0])
	return
}

func Mock{{.BuildClassName}}(pbData {{.Cfg.ValueType}}){
    {{.BuildObjName}}.isLoad = true
    {{.BuildObjName}}.metaData = newMetaData({{ .Cfg.SheetVar }}, pbData)
    tableChanged({{ .Cfg.SheetVar }})
}



func Get{{.BuildClassName}}Copy(ctx interface{}) {{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		val := tab.(*pb.{{.Cfg.ConfigStructName}})
        var ret = depthCopy{{.BuildClassName}}(val)
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           ret = {{.BuildObjName}}.get{{.BuildClassName}}CB(ret)
        }
        return ret
	}
	return nil
}

func Get{{.BuildClassName}}ReadOnly(ctx interface{}) {{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		val := tab.({{.Cfg.ValueType}})
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           val = {{.BuildObjName}}.get{{.BuildClassName}}CB(val)
        }
        return val
	}
	return nil
}

//Get{{.BuildClassName}}Default 返回默认结构体（深拷贝）
func Get{{.BuildClassName}}Default() {{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getDefaultTable()
	if tab != nil {
		val := tab.({{.Cfg.ValueType}})
        var ret = depthCopy{{.BuildClassName}}(val)
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           ret = {{.BuildObjName}}.get{{.BuildClassName}}CB(ret)
        }
        return ret
	}
	return nil
}