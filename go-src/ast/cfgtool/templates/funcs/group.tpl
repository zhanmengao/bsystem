
type TypeGet{{.BuildClassName}}CB func(conf []{{.Cfg.ValueType}})[]{{.Cfg.ValueType}}
type TypeGet{{.BuildClassName}}CBOnNil func({{.Cfg.KeyType}},map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})[]{{.Cfg.ValueType}}
type TypeFilter{{.BuildClassName}}CB func(conf {{.Cfg.ValueType}})bool

var get{{.BuildClassName}}NilCB    TypeGet{{.BuildClassName}}CBOnNil


func load{{.BuildClassName}}(latestMsg proto.Unmarshaler) {
	msg,ok := latestMsg.(*pb.{{.Cfg.AryStructName}})
	if !ok{
        xlog.LevelLogfn(xlog.FATAL, "load table .Cfg.AryStructName proto error")
	    return
	}
	if len(msg.Ary) == 0 {
		return
	}
    m := make(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}}, len(msg.Ary)/2)
    for _,item := range msg.Ary {
        if val,ok := m[item.{{.Cfg.Key}}];!ok{
            val = make([]{{.Cfg.ValueType}},0,1)
            m[item.{{.Cfg.Key}}] = val
        }
        m[item.{{.Cfg.Key}}]  = append(m[item.{{.Cfg.Key}}],item)
    }
    Mock{{.BuildClassName}}(m)
	return
}

func Mock{{.BuildClassName}}(m map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}}){
    {{.BuildObjName}}.isLoad = true
    {{.BuildObjName}}.metaData = newMetaData({{ .Cfg.SheetVar }}, m)
    tableChanged({{ .Cfg.SheetVar }})
}

//Register{{.BuildClassName}}OnNilCB 注册GetByID为空的的回调函数，可用于修正读表返回值
func Register{{.BuildClassName}}OnNilCB(cb TypeGet{{.BuildClassName}}CBOnNil){
    get{{.BuildClassName}}NilCB = cb
}

func Get{{.BuildClassName}}ByIDCopy(ctx interface{},{{.Cfg.Key}} {{.Cfg.KeyType}}) []{{.Cfg.ValueType}} {
	tab := Get{{.BuildClassName}}ByIDReadOnly(ctx, {{.Cfg.Key}})
	if tab != nil {
		tab = depthCopy{{.BuildClassName}}List(tab)
	}
	return tab
}

func Get{{.BuildClassName}}ByIDReadOnly(ctx interface{},{{.Cfg.Key}} {{.Cfg.KeyType}}) []{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		m := tab.(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})
		var ret []{{.Cfg.ValueType}}
		if val,ok := m[{{.Cfg.Key}}];ok{
            ret = val
		}
		if ret == nil && get{{.BuildClassName}}NilCB!=nil{
           cpMap := depthCopy{{.BuildClassName}}Map(m)
           ret = get{{.BuildClassName}}NilCB({{.Cfg.Key}},cpMap)
		}
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           ret = {{.BuildObjName}}.get{{.BuildClassName}}CB(ret)
        }
		return ret
	}
	return nil
}

func Get{{.BuildClassName}}ByIDDefault({{.Cfg.Key}} {{.Cfg.KeyType}}) []{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getDefaultTable()
	if tab != nil {
		m := tab.(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})
		var ret []{{.Cfg.ValueType}}
		if val,ok := m[{{.Cfg.Key}}];ok{
            ret = val
		}
		if ret == nil && get{{.BuildClassName}}NilCB!=nil{
           cpMap := depthCopy{{.BuildClassName}}Map(m)
           ret = get{{.BuildClassName}}NilCB({{.Cfg.Key}},cpMap)
		}
        if {{.BuildObjName}}.get{{.BuildClassName}}CB != nil{
           ret = {{.BuildObjName}}.get{{.BuildClassName}}CB(ret)
        }
		return ret
	}
	return nil
}

func Get{{.BuildClassName}}ByFuncCopy(ctx interface{},cb TypeFilter{{.BuildClassName}}CB) []{{.Cfg.ValueType}} {
    rd := Get{{.BuildClassName}}ByFuncReadOnly(ctx,cb)
    ret := make([]{{.Cfg.ValueType}},len(rd))
    for i,v := range rd{
        ret[i] = depthCopy{{.BuildClassName}}(v)
    }
    return ret
}

//Get{{.BuildClassName}}ByFuncReadOnly 根据回调函数筛选对象，返回筛选后的切片
func Get{{.BuildClassName}}ByFuncReadOnly(ctx interface{},cb TypeFilter{{.BuildClassName}}CB) []{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		m := tab.(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})
		var ret = make([]{{.Cfg.ValueType}},0,1)
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

func Get{{.BuildClassName}}ByFuncDefault(cb TypeFilter{{.BuildClassName}}CB) []{{.Cfg.ValueType}} {
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getDefaultTable()
	if tab != nil {
		m := tab.(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})
		var ret = make([]{{.Cfg.ValueType}},0,1)
		for _,l := range m{
		    for _,v := range l{
		        if cb(v){
		            ret = append(ret, depthCopy{{.BuildClassName}}(v))
		        }
		    }
		}
		return ret
	}
	return nil
}

//Get{{.BuildClassName}}Copy 返回一个拷贝的map
func Get{{.BuildClassName}}Copy(ctx interface{})map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}}{
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getTable(ctx)
	if tab != nil {
		m := tab.(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})
		cpMap := depthCopy{{.BuildClassName}}Map(m)
		return cpMap
	}
	return nil
}

//Get{{.BuildClassName}}Default 返回默认的map（深拷贝）
func Get{{.BuildClassName}}Default()map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}}{
    if !{{.BuildObjName}}.isLoad {
        panic("{{.BuildObjName}} not isLoad")
    }
	tab := {{.BuildObjName}}.metaData.getDefaultTable()
	if tab != nil {
		m := tab.(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})
		cpMap := depthCopy{{.BuildClassName}}Map(m)
		return cpMap
	}
	return nil
}


//depthCopy{{.BuildClassName}}Map 深拷贝Map
func depthCopy{{.BuildClassName}}Map(m map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}})map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}}{
    cpMap := make(map[{{.Cfg.KeyType}}][]{{.Cfg.ValueType}},len(m))
	for k,v := range m{
	    cpMap[k] = depthCopy{{.BuildClassName}}List(v)
	}
	return cpMap
}


//depthCopy{{.BuildClassName}}List 深拷贝List
func depthCopy{{.BuildClassName}}List(arr []{{.Cfg.ValueType}})[]{{.Cfg.ValueType}}{
    cpArray := make([]{{.Cfg.ValueType}},len(arr))
	for i,v := range arr{
	    cpArray[i] = depthCopy{{.BuildClassName}}(v)
	}
	return cpArray
}