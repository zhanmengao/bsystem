package main

import (
	"fmt"
	"regexp"
	"time"
)

var (
	offset         int64
	offsetDuration time.Duration
	reg            = regexp.MustCompile(`[-: /]+`)
	loc            = time.Local
)

const (
	layoutTimeWithoutSeparator = "20060102150405"
	layoutDate                 = "20060102"
	layoutTime                 = "2006-01-02 15:04:05"
	msPerSecond                = 1000
	secondsPerDay              = 86400
	NaturalDay                 = msPerSecond * secondsPerDay
	NaturalWeekDay             = NaturalDay * 7
)

func parseTime(tf string) (t time.Time, err error) {
	str := reg.ReplaceAllString(tf, "")
	if len(str) == len(layoutDate) {
		str += "000000"
	}
	t, err = time.ParseInLocation(layoutTimeWithoutSeparator, str, loc)
	return
}

//格式： yyyy-mm-dd hh:MM:ss| yyyy-mm-dd
func ParseToTimeUnixMS(tf string) int64 {
	t, err := parseTime(tf)
	if err != nil {
		return -1
	}
	return t.UnixNano() / 1e6
}

//@deprecated
func ParseToTimeUnix(tf string) int64 {
	t, err := parseTime(tf)
	if err != nil {
		return -1
	}
	return t.Unix()
}

//获取当前时间戳（毫秒）
func Millisecond() int64 {
	return time.Now().UnixNano()/1e6 + offset
}

//Unix 获取当前时间戳（秒）
func Unix() int64 {
	return Millisecond() / 1000
}

//给定的时间戳，计算当天零点的时间戳
func DayZeroMillisecond(ms int64) int64 {
	ft := layoutDate
	t := time.Unix(ms/msPerSecond, 0).In(loc)
	val := t.Format(ft)
	t, _ = time.ParseInLocation(ft, val, loc)
	return t.UnixNano() / 1e6
}

func TimeMSToString(ms int64) string {
	return TimeToString(ms / msPerSecond)
}

func TimeToString(sec int64) string {
	return time.Unix(sec, 0).In(loc).Format(layoutTime)
}

func MSToLocalTimeString(ms int64) string {
	return time.Unix(ms/msPerSecond, 0).Format(layoutTime)
}

func TimeMsToDateString(ms int64) string {
	return time.Unix(ms/msPerSecond, 0).In(loc).Format(layoutDate)
}

func Data(ms int64) (year int, month, day int) {
	y, m, d := time.Unix(ms/msPerSecond, 0).In(loc).Date()
	year = y
	month = int(m)
	day = d
	return
}

func IsSameDayMs(ms1, ms2 int64) bool {
	return IsSameDay(ms1/msPerSecond, ms2/msPerSecond)
}

func IsSameDay(d1, d2 int64) bool {
	t1 := time.Unix(d1, 0).In(loc)
	t2 := time.Unix(d2, 0).In(loc)
	y1, m1, dd1 := t1.Date()
	y2, m2, dd2 := t2.Date()
	return y1 == y2 && m1 == m2 && dd1 == dd2
}

//n天之后的0点时间(毫秒)
func DaysZeroTimeMs(n int) int64 {
	ms := Millisecond() + int64(n*secondsPerDay*msPerSecond)
	return DayZeroMillisecond(ms)
}

//本周第某天零点时间（毫秒）,每周从周日开始算起
func WeekDayZeroTimeMs(day time.Weekday) int64 {
	if day > time.Saturday || day < time.Sunday {
		return -1
	}
	t := time.Now().In(loc)
	if offsetDuration != 0 {
		t = t.Add(offsetDuration)
	}
	today := t.Weekday()
	delta := day - today
	return DaysZeroTimeMs(int(delta))
}

//当前WeekDay到目标星期几还要几天
func NextPeriodWeekDayZeroTimeMs(day time.Weekday) int64 {
	t := time.Now().In(loc)
	if offsetDuration != 0 {
		t = t.Add(offsetDuration)
	}
	today := t.Weekday()
	if today == day {

		return DaysZeroTimeMs(7)
	} else {
		delta := (day - today + 7) % 7
		return DaysZeroTimeMs(int(delta))
	}
}

//几个月之后的某一天零时
//month为第n个月之后（前）,0为本月，正数为之后，负数为之前
//date 第几天
func MonthDayZeroTime(month, date int) int64 {
	t := time.Now().In(loc)
	if offsetDuration != 0 {
		t = t.Add(offsetDuration)
	}
	t = t.AddDate(0, month, 0)
	y, m, _ := t.Date()
	str := fmt.Sprintf("%d-%02d-%02d 00:00:00", y, m, date)
	return ParseToTimeUnixMS(str)
}

func WeekDay(ms int64) (weekDay time.Weekday) {
	weekDay = time.Unix(ms/msPerSecond, 0).In(loc).Weekday()
	return
}
