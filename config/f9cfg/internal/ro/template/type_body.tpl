
}

func Get{{.ProtoType}}ReadOnly(m *{{.ProtoType}})*{{.ROType}}{
	return (*{{.ROType}})(unsafe.Pointer(m))
}

func (m *{{.ProtoType}})GetReadOnly() *{{.ROType}}{
    return Get{{.ProtoType}}ReadOnly(m)
}

func(p *{{.ROType}})GetOriginalConfig()*{{.ProtoType}}{
    tmp := (*{{.ProtoType}})(unsafe.Pointer(p))
    data ,_ := tmp.Marshal()
    ret := &{{.ProtoType}}{}
    ret.Unmarshal(data)
    return ret
}

func(p *{{.ROType}})String()string{
    if p == nil{
        return ""
    }
    o := (*{{.ProtoType}})(unsafe.Pointer(p))
    return o.String()
}

{{ range $FilterName,$value := .NormalList }}
    func(p *{{$.ROType}})Get{{$FilterName}}(){{$value.FilterType}}{
    	return p.ro{{$FilterName}}
    }
{{ end }}
{{ range $FilterName,$value := .StructList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()*{{$value.ROType}}{
    	return p.ro{{$FilterName}}
    }
{{ end }}
{{ range $FilterName,$value := .NormalSliceList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()[]{{$value.FilterType}}{
        ret := make([]{{$value.FilterType}},len(p.ro{{$FilterName}}))
        copy(ret,p.ro{{$FilterName}})
    	return ret
    }
{{ end }}
{{ range $FilterName,$value := .StructSliceList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()[]*{{$value.ROType}}{
        ret := make([]*{{$value.ROType}},len(p.ro{{$FilterName}}))
        copy(ret,p.ro{{$FilterName}})
    	return ret
    }
{{ end }}
{{ range $FilterName,$value := .NormalMapList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()map[{{$value.KeyType}}]{{$value.ValueType}}{
        ret := make(map[{{$value.KeyType}}]{{$value.ValueType}},len(p.ro{{$FilterName}}))
        for k,v := range p.ro{{$FilterName}}{
            ret[k]=v
        }
    	return ret
    }
{{ end }}
{{ range $FilterName,$value := .StructMapList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()map[{{$value.KeyType}}]*{{$value.ROType}}{
        ret := make(map[{{.KeyType}}]*{{$value.ROType}},len(p.ro{{$FilterName}}))
        for k,v := range p.ro{{$FilterName}}{
            ret[k]=v
        }
    	return ret
    }
{{ end }}
{{range $FilterName := .SelfList}}
    func(p *{{$.ROType}})Get{{$FilterName}}()*{{$.ROType}}{
    	return p.ro{{$FilterName}}
    }
{{ end }}
{{range $FilterName := .SelfSliceList}}
    func(p *{{$.ROType}})Get{{$FilterName}}()[]*{{$.ROType}}{
        ret := make([]*{{$.ROType}},len(p.ro{{$FilterName}}))
        copy(ret,p.ro{{$FilterName}})
    	return ret
    }
{{ end }}
{{range $FilterName,$KeyType := .SelfMapList}}
    func(p *{{$.ROType}})Get{{$FilterName}}()map[{{$KeyType}}]*{{$.ROType}}{
        ret := make(map[{{$KeyType}}]*{{$.ROType}},len(p.ro{{$FilterName}}))
        for k,v := range p.ro{{$FilterName}}{
            ret[k]=v
        }
    	return ret
    }
 {{ end }}