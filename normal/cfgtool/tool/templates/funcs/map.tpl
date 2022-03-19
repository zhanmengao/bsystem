// {{.FuncName}} {{.Desc}}; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}(ctx contextdefine.IContext) (m map[{{.KeyType}}]{{.ValueType}}) {
    // 深度拷贝
    ary := &pb.{{.AryStructName}}{}
    c.sheetCfg{{.Sheet}}.GetObj(ctx, ary)

    m = make(map[{{.KeyType}}]{{.ValueType}}, len(ary.Ary)/2)
    for _,item := range ary.Ary {
        m[item.{{.Key}}] = item
    }
    return
}

// Deprecated; 指针传递需要小心被修改; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) direct{{.FuncName}}(ctx contextdefine.IContext) (m map[{{.KeyType}}]{{.ValueType}}) {
    _,obj := c.sheetCfg{{.Sheet}}.GetDirect(ctx)
    ary := obj.(*pb.{{.AryStructName}})

    m = make(map[{{.KeyType}}]{{.ValueType}}, len(ary.Ary)/2)
    for _,item := range ary.Ary {
        m[item.{{.Key}}] = item
    }
    return
}

// Len{{.FuncName}} {{.Desc}} 不保证后续其他方法操作的原子性; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) Len{{.FuncName}}(ctx contextdefine.IContext) int {
	return len(c.direct{{.FuncName}}(ctx))
}

// In{{.FuncName}} {{.Desc}} 不保证后续其他方法操作的原子性; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) In{{.FuncName}}(ctx contextdefine.IContext, _{{.KeyLower}} {{.KeyType}}) (ok bool) {
     m := c.direct{{.FuncName}}(ctx)
     _,ok = m[_{{.KeyLower}}]
	return
}

// {{.FuncName}}Item {{.Desc}}; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}Item(ctx contextdefine.IContext, _{{.KeyLower}} {{.KeyType}}) (item {{.ValueType}}, ok bool) {
	//复制
	m := c.{{.FuncName}}(ctx)
	item,ok = m[_{{.KeyLower}}]
	if !ok {
		return
	}

	return
}

// Deprecated; 指针传递需要小心被修改; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) direct{{.FuncName}}Item(ctx contextdefine.IContext, _{{.KeyLower}} {{.KeyType}}) (item {{.ValueType}}, ok bool) {
	//复制
	m := c.{{.FuncName}}(ctx)
	item,ok = m[_{{.KeyLower}}]
	if !ok {
		return
	}

	return
}