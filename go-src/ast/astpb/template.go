package main

import (
	"bytes"
	"errors"
	"fmt"
	"go/format"
	"io"
	"log"
	"os"
	"path"
	"path/filepath"
	"runtime"
	"text/template"
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
	var rootDir string
	if _, file, _, ok := runtime.Caller(0); ok {
		rootDir, _ = filepath.Split(file)
	}
	if head, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/head.tpl"))); err != nil {
		return
	}
	if typeHeadTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/type_head.tpl"))); err != nil {
		return
	}
	if typeBodyTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/type_body.tpl"))); err != nil {
		return
	}
	if normalTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/normal.tpl"))); err != nil {
		return
	}
	if structTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/struct.tpl"))); err != nil {
		return
	}
	if normalListTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/normal_list.tpl"))); err != nil {
		return
	}
	if structListTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/struct_list.tpl"))); err != nil {
		return
	}
	if normalMapTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/normal_map.tpl"))); err != nil {
		return
	}
	if structMapTemplate, err = template.ParseFiles(path.Join(rootDir, fmt.Sprintf("template/struct_map.tpl"))); err != nil {
		return
	}
	pf, err := os.OpenFile(output, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		return
	}
	defer pf.Close()
	b := bytes.NewBufferString("")
	err = head.Execute(b, s)
	if err != nil {
		return
	}
	if _, err = pf.Write(b.Bytes()); err != nil {
		return
	}
	for _, v := range s.MetaList {
		if err = genTemplate(v, pf); err != nil {
			return
		}
	}

	return
}

//为一个meta生成RO对象
func genTemplate(meta *roMeta, pf io.Writer) (err error) {
	b := bytes.NewBufferString("")
	if _, exist := s.AlreadyGen[meta.ProtoType]; exist {
		return
	}
	s.AlreadyGen[meta.ProtoType] = struct{}{}
	//先定义类型
	if err = typeHeadTemplate.Execute(b, meta); err != nil {
		err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
		return
	}
	//定义成员
	for _, k := range meta.FilterList {
		if v, exist := meta.NormalList[k]; exist {
			info := &genInfo{
				FilterType: v.FilterType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = normalTemplate.Execute(b, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.StructList[k]; exist {
			info := &genInfo{
				FilterType: v.ROType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = structTemplate.Execute(b, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.NormalSliceList[k]; exist {
			info := &genInfo{
				FilterType: v.FilterType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = normalListTemplate.Execute(b, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.StructSliceList[k]; exist {
			info := &genInfo{
				FilterType: v.ROType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = structListTemplate.Execute(b, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.NormalMapList[k]; exist {
			info := &genInfo{
				KeyType:    v.KeyType,
				FilterType: v.ValueType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = normalMapTemplate.Execute(b, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		if v, exist := meta.StructMapList[k]; exist {
			info := &genInfo{
				KeyType:    v.KeyType,
				FilterType: v.ROType,
				FilterName: k,
				ROType:     meta.ROType,
			}
			if err = structMapTemplate.Execute(b, info); err != nil {
				err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
				return
			}
			continue
		}
		for _, v := range meta.SelfList {
			if v == k {
				info := &genInfo{
					FilterType: meta.ROType,
					FilterName: k,
					ROType:     meta.ROType,
				}
				if err = structTemplate.Execute(b, info); err != nil {
					err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
					return
				}
				continue
			}
		}
		for _, v := range meta.SelfSliceList {
			if v == k {
				info := &genInfo{
					FilterType: meta.ROType,
					FilterName: k,
					ROType:     meta.ROType,
				}
				if err = structListTemplate.Execute(b, info); err != nil {
					err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
					return
				}
				continue
			}
		}
		for filterName, KeyType := range meta.SelfMapList {
			if filterName == k {
				info := &genInfo{
					FilterType: meta.ROType,
					FilterName: k,
					ROType:     meta.ROType,
					KeyType:    KeyType,
				}
				if err = structMapTemplate.Execute(b, info); err != nil {
					err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
					return
				}
				continue
			}
		}
	}
	//定义函数
	if err = typeBodyTemplate.Execute(b, meta); err != nil {
		err = errors.New(fmt.Sprintf("type = %s ,err = %s \n", meta.ProtoType, err.Error()))
		return
	}
	var buf []byte
	if buf, err = format.Source(b.Bytes()); err != nil {
		log.Println(b.String())
		return
	}
	if _, err = pf.Write(buf); err != nil {
		return
	}
	//递归定义自己的成员
	for _, v := range meta.StructSliceList {
		if err = genTemplate(v, pf); err != nil {
			return
		}
	}
	for _, v := range meta.StructList {
		if err = genTemplate(v, pf); err != nil {
			return
		}
	}
	return
}
