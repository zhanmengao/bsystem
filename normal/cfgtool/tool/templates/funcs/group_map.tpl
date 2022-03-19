// {{.FuncName}} {{.Desc}}; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}(ctx contextdefine.IContext) (m map[{{.KeyType}}]map[{{.SubKeyType}}]{{.ValueType}}) {
    // 深度拷贝
    ary := &pb.{{.AryStructName}}{}
    c.sheetCfg{{.Sheet}}.GetObj(ctx, ary)

    m = make(map[{{.KeyType}}]map[{{.SubKeyType}}]{{.ValueType}}, len(ary.Ary)/2)
    for _, item := range ary.Ary {
        if m[item.{{.Key}}] == nil {
            m[item.{{.Key}}] = make(map[{{.SubKeyType}}]{{.ValueType}})
        }
        m[item.{{.Key}}][item.{{.SubKey}}] = item
    }
    return
}

// Deprecated; 指针传递需要小心被修改; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) direct{{.FuncName}}(ctx contextdefine.IContext) (m map[{{.KeyType}}]map[{{.SubKeyType}}]{{.ValueType}}) {
    _,obj := c.sheetCfg{{.Sheet}}.GetDirect(ctx)
    ary := obj.(*pb.{{.AryStructName}})

    m = make(map[{{.KeyType}}]map[{{.SubKeyType}}]{{.ValueType}}, len(ary.Ary)/2)
    for _, item := range ary.Ary {
        if m[item.{{.Key}}] == nil {
            m[item.{{.Key}}] = make(map[{{.SubKeyType}}]{{.ValueType}})
        }
        m[item.{{.Key}}][item.{{.SubKey}}] = item
    }

    return
}

// Len{{.FuncName}} {{.Desc}} 不保证后续其他方法操作的原子性; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) Len{{.FuncName}}(ctx contextdefine.IContext) int {
    return len(c.direct{{.FuncName}}(ctx))
}

// IsIn{{.FuncName}} {{.Desc}} 不保证后续其他方法操作的原子性; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) In{{.FuncName}}(ctx contextdefine.IContext, _{{.KeyLower}} {{.KeyType}}) (ok bool) {
    m := c.direct{{.FuncName}}(ctx)
    _,ok = m[_{{.KeyLower}}]
	return
}

// GetItem{{.FuncName}} {{.Desc}}; 获取{{.Key}}对应map[{{.SubKeyType}}]{{.ValueType}}; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}ItemByKey(ctx contextdefine.IContext, _{{.KeyLower}} {{.KeyType}}) (item map[{{.SubKeyType}}]{{.ValueType}}, ok bool) {
	//复制
	m := c.{{.FuncName}}(ctx)
	item,ok = m[_{{.KeyLower}}]
	if !ok {
		return
	}

	return
}

// {{.FuncName}}ItemBySubKey; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}ItemBySubKey(ctx contextdefine.IContext, _{{.KeyLower}} {{.KeyType}}, _{{.SubKeyLower}} {{.SubKeyType}}) (item {{.ValueType}},ok bool) {
    m := c.{{.FuncName}}(ctx)
    tmp,ok := m[_{{.KeyLower}}]
    if !ok {
        return
    }
    item,ok = tmp[_{{.SubKeyLower}}]
    return
}