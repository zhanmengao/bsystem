package dirtyfilter

import (
	"bufio"
	"bytes"
	"unicode"

	"io"
	"os"
	"strings"
)

type DirtyFilter interface {
	Filter(text string, excludes ...rune) ([]string, error)
	Replace(text string, delim rune) (string, error)
}

func NewFileDirtFilter(file string) (filter DirtyFilter, err error) {
	fp, err := os.Open(file)
	if err != nil {
		return
	}
	defer fp.Close()
	bio := bufio.NewReader(fp)
	var line string
	var bufl []byte
	nf := &nodeFilter{root: newNode()}
	for {
		bufl, _, err = bio.ReadLine()
		if err == io.EOF {
			err = nil
			break
		}
		if err != nil {
			return
		}

		line = strings.Trim(string(bufl), "\r\n \t")
		if line == "" {
			continue
		}
		pts := strings.Split(line, "|")
		line = pts[0]
		nf.addDirtyWords(line)
	}
	filter = nf
	return
}

func newNode() *node {
	return &node{
		child: make(map[rune]*node),
	}
}

type node struct {
	end   bool
	child map[rune]*node
}

type nodeFilter struct {
	root          *node
	isFirstLetter bool
	isFirstDigit  bool
}

func (nf *nodeFilter) addDirtyWords(text string) {
	n := nf.root
	uchars := []rune(text)
	for i, l := 0, len(uchars); i < l; i++ {
		if unicode.IsSpace(uchars[i]) {
			continue
		}
		if _, ok := n.child[uchars[i]]; !ok {
			n.child[uchars[i]] = newNode()
		}
		n = n.child[uchars[i]]
	}
	n.end = true
}

func (nf *nodeFilter) Filter(text string, excludes ...rune) ([]string, error) {
	buf := bytes.NewBufferString(text)
	defer buf.Reset()
	return nf.FilterReader(buf, excludes...)
}

func (nf *nodeFilter) FilterResult(text string, excludes ...rune) (map[string]int, error) {
	buf := bytes.NewBufferString(text)
	defer buf.Reset()
	return nf.FilterReaderResult(buf, excludes...)
}

func (nf *nodeFilter) FilterReader(reader io.Reader, excludes ...rune) ([]string, error) {
	data, err := nf.FilterReaderResult(reader, excludes...)
	if err != nil {
		return nil, err
	}
	var result []string
	for k := range data {
		result = append(result, k)
	}
	return result, nil
}

func (nf *nodeFilter) FilterReaderResult(reader io.Reader, excludes ...rune) (map[string]int, error) {
	var (
		uchars []rune
	)
	data := make(map[string]int)
	bi := bufio.NewReader(reader)
	for {
		ur, _, err := bi.ReadRune()
		if err != nil {
			if err != io.EOF {
				return nil, err
			}
			break
		}
		if nf.checkExclude(ur, excludes...) {
			continue
		}
		if (unicode.IsSpace(ur) || unicode.IsPunct(ur)) && len(uchars) > 0 {
			nf.doFilter(uchars[:], data)
			uchars = nil
			continue
		}
		uchars = append(uchars, ur)
	}
	if len(uchars) > 0 {
		nf.doFilter(uchars, data)
	}
	return data, nil
}

func (nf *nodeFilter) Replace(text string, delim rune) (string, error) {
	uchars := []rune(text)
	idexs := nf.doIndexes(uchars)
	if len(idexs) == 0 {
		// return "", nil
		return text, nil
	}
	for i := 0; i < len(idexs); i++ {
		uchars[idexs[i]] = rune(delim)
	}
	return string(uchars), nil
}

func (nf *nodeFilter) checkExclude(u rune, excludes ...rune) bool {
	if len(excludes) == 0 {
		return false
	}
	var exist bool
	for i, l := 0, len(excludes); i < l; i++ {
		if u == excludes[i] {
			exist = true
			break
		}
	}
	return exist
}

func (nf *nodeFilter) doFilter(uchars []rune, data map[string]int) {
	var result []string
	ul := len(uchars)
	buf := new(bytes.Buffer)
	n := nf.root
	for i := 0; i < ul; i++ {
		nf.resetFlag()
		if _, ok := n.child[uchars[i]]; !ok {
			continue
		}
		nf.setFlag(uchars[i])

		n = n.child[uchars[i]]
		buf.WriteRune(uchars[i])
		if n.end {
			result = append(result, buf.String())
		}
		for j := i + 1; j < ul; j++ {
			buf.WriteRune(uchars[j])
			if nf.isSkipList(uchars[j]) {
				result = append(result, buf.String())
				continue
			}
			if _, ok := n.child[uchars[j]]; !ok {
				break
			}
			n = n.child[uchars[j]]
			buf.WriteRune(uchars[j])
			if n.end {
				result = append(result, buf.String())
			}
		}
		buf.Reset()
		n = nf.root
	}
	for i, l := 0, len(result); i < l; i++ {
		var c int
		if v, ok := data[result[i]]; ok {
			c = v
		}
		data[result[i]] = c + 1
	}
}

func (nf *nodeFilter) doIndexes(uchars []rune) (idexs []int) {
	var (
		tIdexs = make([]int, 0, 8)
		ul     = len(uchars)
		n      = nf.root
	)
	idexs = make([]int, 0, len(uchars))
	for i := 0; i < ul; i++ {
		nf.resetFlag()
		uci := uchars[i]
		if _, ok := n.child[uci]; !ok {
			continue
		}
		nf.setFlag(uci)

		n = n.child[uci]
		tIdexs = append(tIdexs, i)
		if n.end {
			idexs = nf.appendTo(idexs, tIdexs)
			tIdexs = tIdexs[:0]
		}
		for j := i + 1; j < ul; j++ {
			ucj := uchars[j]
			if nf.isSkipList(ucj) {
				tIdexs = append(tIdexs, j)
				continue
			}
			if _, ok := n.child[ucj]; !ok {
				break
			}
			n = n.child[ucj]
			tIdexs = append(tIdexs, j)
			if n.end {
				idexs = nf.appendTo(idexs, tIdexs)
			}
		}
		if len(tIdexs) > 0 {
			tIdexs = tIdexs[:0]
		}
		n = nf.root
	}
	return
}

func (nf *nodeFilter) appendTo(dst, src []int) []int {
	return append(dst, src...)
}

func isDigit(r rune) bool {
	if r > 0xff {
		return false
	}
	c := byte(r)
	return c >= '0' && c <= '9'
}

func isAsciiChar(r rune) bool {
	if r > 0xff {
		return false
	}
	c := byte(r)
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
}

func (nf *nodeFilter) isSkipList(r rune) bool {
	if unicode.IsSpace(r) ||
		isDigit(r) ||
		unicode.IsPunct(r) {
		return true
	}
	if !nf.isFirstLetter {
		if isAsciiChar(r) {
			return true
		}
	}
	return false
}

func (nf *nodeFilter) resetFlag() {
	nf.isFirstLetter = false
	nf.isFirstDigit = false
}

func (nf *nodeFilter) setFlag(r rune) {
	if isDigit(r) {
		nf.isFirstDigit = true
	}
	if isAsciiChar(r) {
		nf.isFirstLetter = true
	}
}
