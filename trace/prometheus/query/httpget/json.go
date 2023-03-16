package httpget

import (
	"encoding/json"
	"fmt"
	simplejson "github.com/bitly/go-simplejson"
	"io/ioutil"
	"net/http"
	//"reflect"
	"regexp"
	"strconv"
	"strings"
)

//const blkSize int = 10000

type trend struct {
	date    int64
	last_px float32 //最新价
	avg_px  float32 //平均价
	volumn  float32 //成交量
}

var (
	lines   []string
	blksLen []int
	isGB    bool
)

func check(err error) {
	if err != nil {
		panic(err.Error())
	}
}

func Get(url string) ([]byte, error) {
	defer func() {
		if err := recover(); err != nil {
			fmt.Println(err)
		}
	}()
	resp, err := http.Get(url)
	check(err)
	//Println(resp.StatusCode)
	if resp.StatusCode != 200 {
		panic("FUCK")
	}
	return ioutil.ReadAll(resp.Body)
}

func strip(src string) string {
	src = strings.ToLower(src)
	re, _ := regexp.Compile(`<!doctype.*?>`)
	src = re.ReplaceAllString(src, "")

	re, _ = regexp.Compile(`<!--.*?-->`)
	src = re.ReplaceAllString(src, "")

	re, _ = regexp.Compile(`<script[\S\s]+?</script>`)
	src = re.ReplaceAllString(src, "")

	re, _ = regexp.Compile(`<style[\S\s]+?</style>`)
	src = re.ReplaceAllString(src, "")

	re, _ = regexp.Compile(`<.*?>`)
	src = re.ReplaceAllString(src, "")

	re, _ = regexp.Compile(`&.{1,5};|&#.{1,5};`)
	src = re.ReplaceAllString(src, "")

	src = strings.Replace(src, "\r\n", "\n", -1)
	src = strings.Replace(src, "\r", "\n", -1)
	return src
}

func Do(url string) string {
	body, err := Get(url)
	check(err)
	plainText := strip(string(body))
	return plainText
}

func main() {
	str := Do("http://xxx:8081/quote/v1/trend?prod_code=600570.SS&fields=last_px,business_amount,avg_px")
	js, err := simplejson.NewJson([]byte(str))
	check(err)
	arr, _ := js.Get("data").Get("trend").Get("600570.ss").Array()
	t := len(arr)
	stockdata := trend{}
	trends := make([]trend, 0, t)
	for _, v := range arr {
		//就在这里i进行类型判断
		value, _ := v.([]interface{})
		for k, u := range value {

			x, _ := u.(json.Number)                   //类型断言
			y, _ := strconv.ParseFloat(string(x), 64) //将字符型号转化为float64
			//v := reflect.ValueOf(k)
			//fmt.Println("type:", v.Type())
			switch k {
			case 0:
				stockdata.date = int64(y)
			case 1:
				stockdata.last_px = float32(y)
			case 2:
				stockdata.volumn = float32(y)
			case 3:
				stockdata.avg_px = float32(y)
			default:
				fmt.Println("结构体中不存在此元素")

			}

		}
		trends = append(trends, stockdata)

	}
	fmt.Println(trends)
}
