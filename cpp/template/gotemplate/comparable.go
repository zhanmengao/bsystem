package gotemplate

// ComparableMap 要求KEY可比较
type ComparableMap[KEY comparable, VALUE any] map[KEY]VALUE
