
}

func Get{{.ProtoType}}ReadOnly(m *{{.ProtoType}})*{{.ROType}}{
	return (*{{.ROType}})(unsafe.Pointer(m))
}
{{ range $FilterName,$value := .NormalList }}
    func(p *{{$.ROType}})Get{{$FilterName}}(){{$value.FilterType}}{
    	return p._{{$FilterName}}
    }
{{ end }}
{{ range $FilterName,$value := .StructList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()*{{$value.ROType}}{
    	return p._{{$FilterName}}
    }
{{ end }}
{{ range $FilterName,$value := .NormalSliceList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()[]{{$value.FilterType}}{
        ret := make([]{{$value.FilterType}},len(p._{{$FilterName}}))
        copy(ret,p._{{$FilterName}})
    	return ret
    }
{{ end }}
{{ range $FilterName,$value := .StructSliceList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()[]*{{$value.ROType}}{
        ret := make([]*{{$value.ROType}},len(p._{{$FilterName}}))
        copy(ret,p._{{$FilterName}})
    	return ret
    }
{{ end }}
{{ range $FilterName,$value := .NormalMapList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()map[{{$value.KeyType}}]{{$value.ValueType}}{
        ret := make(map[{{$value.KeyType}}]{{$value.ValueType}},len(p._{{$FilterName}}))
        for k,v := range p._{{$FilterName}}{
            ret[k]=v
        }
    	return ret
    }
{{ end }}
{{ range $FilterName,$value := .StructMapList }}
    func(p *{{$.ROType}})Get{{$FilterName}}()map[{{$value.KeyType}}]*{{$value.ROType}}{
        ret := make(map[{{.KeyType}}]*{{$value.ROType}},len(p._{{$FilterName}}))
        for k,v := range p._{{$FilterName}}{
            ret[k]=v
        }
    	return ret
    }
{{ end }}
{{range $FilterName := .SelfList}}
    func(p *{{$.ROType}})Get{{$FilterName}}()*{{$.ROType}}{
    	return p._{{$FilterName}}
    }
{{ end }}
{{range $FilterName := .SelfSliceList}}
    func(p *{{$.ROType}})Get{{$FilterName}}()[]*{{$.ROType}}{
        ret := make([]*{{$.ROType}},len(p._{{$FilterName}}))
        copy(ret,p._{{$FilterName}})
    	return ret
    }
{{ end }}
{{range $FilterName,$KeyType := .SelfMapList}}
    func(p *{{$.ROType}})Get{{$FilterName}}()map[{{$KeyType}}]*{{$.ROType}}{
        ret := make(map[{{$KeyType}}]*{{$.ROType}},len(p._{{$FilterName}}))
        for k,v := range p._{{$FilterName}}{
            ret[k]=v
        }
    	return ret
    }
 {{ end }}