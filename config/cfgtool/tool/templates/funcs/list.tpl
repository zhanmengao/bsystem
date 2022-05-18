// {{.FuncName}} {{.Desc}}; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}(ctx contextdefine.IContext) (l []{{.ValueType}}) {
    // 深度拷贝
    ary := &pb.{{.AryStructName}}{}
    c.sheetCfg{{.Sheet}}.GetObj(ctx, ary)

    l = ary.Ary

    return
}

// Deprecated; 指针传递需要小心被修改; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) direct{{.FuncName}}(ctx contextdefine.IContext) (l []{{.ValueType}}) {
    _,obj := c.sheetCfg{{.Sheet}}.GetDirect(ctx)
    ary := obj.(*pb.{{.AryStructName}})

    l = ary.Ary

    return
}

// Len{{.FuncName}} {{.Desc}} 不保证后续其他方法操作的原子性; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) Len{{.FuncName}}(ctx contextdefine.IContext) int {
	return len(c.direct{{.FuncName}}(ctx))
}