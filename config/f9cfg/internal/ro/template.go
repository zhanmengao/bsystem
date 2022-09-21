package ro

import (
	"bytes"
	"config/f9cfg/internal/meta"
	"errors"
	"fmt"
	"go/format"
	"os"
	"text/template"
)

const (
	defaultSrcBufferSize = 1 * 1024 * 1024
)

var (
	head               *template.Template
	typeHeadTemplate   *template.Template
	typeBodyTemplate   *template.Template
	normalTemplate     *template.Template
	structTemplate     *template.Template
	normalListTemplate *template.Template
	structListTemplate *template.Template
	normalMapTemplate  *template.Template
	structMapTemplate  *template.Template
)

func genTemplateList(output string) (err error) {
	//var rootDir string
	//if _, file, _, ok := runtime.Caller(0); ok {
	//	rootDir, _ = filepath.Split(file)
	//}
	if head, err = loadTpl("template/head.tpl"); err != nil {
		return
	}
	if typeHeadTemplate, err = loadTpl("template/type_head.tpl"); err != nil {
		return
	}
	if typeBodyTemplate, err = loadTpl("template/type_body.tpl"); err != nil {
		return
	}
	if normalTemplate, err = loadTpl("template/normal.tpl"); err != nil {
		return
	}
	if structTemplate, err = loadTpl("template/struct.tpl"); err != nil {
		return
	}
	if normalListTemplate, err = loadTpl("template/normal_list.tpl"); err != nil {
		return
	}
	if structListTemplate, err = loadTpl("template/struct_list.tpl"); err != nil {
		return
	}
	if normalMapTemplate, err = loadTpl("template/normal_map.tpl"); err != nil {
		return
	}
	if structMapTemplate, err = loadTpl("template/struct_map.tpl"); err != nil {
		return
	}
	pf, err := os.OpenFile(output, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		return
	}
	defer pf.Close()
	//默认new 1M的内存出来，提升效率
	bb := bytes.NewBuffer(make([]byte, 0, defaultSrcBufferSize))
	err = head.Execute(bb, s)
	if err != nil {
		return
	}
	//if _, err = pf.Write(bb.Bytes()); err != nil {
	//	return
	//}
	for _, v := range s.MetaList {
		if err = genTemplate(v, bb); err != nil {
			return
		}
	}
	buf, err := format.Source(bb.Bytes())
	if err != nil {
		err = errors.New(fmt.Sprintf("fail to format go source code,err:%s", err.Error()))
		return
	}
	sz, err := pf.Write(buf)
	if err != nil {
		return
	}
	if sz != len(buf) {
		err = errors.New(fmt.Sprintf("fail to write data, size not correct, buf size:%d, written:%d", len(buf), sz))
		return
	}

	return
}

//为一个meta生成RO对象
func genTemplate(meta *meta.RoMeta, bb *bytes.Buffer) (err error) {
	//b := bytes.NewBufferString("")
	if _, exist := s.AlreadyGen[meta.ProtoType]; exist {
		return
	}
	s.AlreadyGen[meta.ProtoType] = struct{}{}
	//先定义类型
	if err = typeHeadTemplate.Execute(bb, meta); err != nil {
		err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
		return
	}
	//定义成员
	for _, k := range meta.FilterList {
		if v, exist := meta.NormalList[k]; exist {
			info := &GenInfo{
				FilterType: v.FilterType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = normalTemplate.Execute(bb, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.StructList[k]; exist {
			info := &GenInfo{
				FilterType: v.ROType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = structTemplate.Execute(bb, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.NormalSliceList[k]; exist {
			info := &GenInfo{
				FilterType: v.FilterType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = normalListTemplate.Execute(bb, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.StructSliceList[k]; exist {
			info := &GenInfo{
				FilterType: v.ROType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = structListTemplate.Execute(bb, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.NormalMapList[k]; exist {
			info := &GenInfo{
				KeyType:    v.KeyType,
				FilterType: v.ValueType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = normalMapTemplate.Execute(bb, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.StructMapList[k]; exist {
			info := &GenInfo{
				KeyType:    v.KeyType,
				FilterType: v.ROType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = structMapTemplate.Execute(bb, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		for _, v := range meta.SelfList {
			if v == k {
				info := &GenInfo{
					FilterType: meta.ROType,
					FilterName: k,
					ROType:     meta.ROType,
				}
				if err = structTemplate.Execute(bb, info); err != nil {
					err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
					return
				}
				continue
			}
		}
		for _, v := range meta.SelfSliceList {
			if v == k {
				info := &GenInfo{
					FilterType: meta.ROType,
					FilterName: k,
					ROType:     meta.ROType,
				}
				if err = structListTemplate.Execute(bb, info); err != nil {
					err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
					return
				}
				continue
			}
		}
		for filterName, KeyType := range meta.SelfMapList {
			if filterName == k {
				info := &GenInfo{
					FilterType: meta.ROType,
					FilterName: k,
					ROType:     meta.ROType,
					KeyType:    KeyType,
				}
				if err = structMapTemplate.Execute(bb, info); err != nil {
					err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
					return
				}
				continue
			}
		}
	}
	//定义函数
	if err = typeBodyTemplate.Execute(bb, meta); err != nil {
		err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
		return
	}
	//var buf []byte
	//if buf, err = format.Source(b.Bytes()); err != nil {
	//	log.Println(b.String())
	//	return
	//}
	//if _, err = bb.Write(buf); err != nil {
	//	return
	//}
	//递归定义自己的成员
	for _, v := range meta.StructSliceList {
		if err = genTemplate(v, bb); err != nil {
			return
		}
	}
	for _, v := range meta.StructList {
		if err = genTemplate(v, bb); err != nil {
			return
		}
	}
	return
}
