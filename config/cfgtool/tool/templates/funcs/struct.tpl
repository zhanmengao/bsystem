// {{.FuncName}} {{.Desc}}; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) {{.FuncName}}(ctx contextdefine.IContext) (obj *pb.{{.ConfigStructName}}) {
    // 深度拷贝
    ary := &pb.{{.AryStructName}}{}
    c.sheetCfg{{.Sheet}}.GetObj(ctx, ary)

    for _, c := range ary.Ary {
        obj = c
    }

    if obj == nil { // 如果配置表填空传递的是空, 正常不会出现
        obj = &pb.{{.ConfigStructName}}{}
    }
    return
}

// Deprecated; 指针传递需要小心被修改; 传入调用方ctx参数, 如果不存在则传nil
func (c *{{.BuildClassName}}) direct{{.FuncName}}(ctx contextdefine.IContext) (obj *pb.{{.ConfigStructName}}) {
    _,aryObj := c.sheetCfg{{.Sheet}}.GetDirect(ctx)
    ary := aryObj.(*pb.{{.AryStructName}})

    for _, c := range ary.Ary {
        obj = c
    }

    if obj == nil { // 如果配置表填空传递的是空, 正常不会出现
        obj = &pb.{{.ConfigStructName}}{}
    }
    return
}