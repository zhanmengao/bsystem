package httpget

import (
	"encoding/json"
	"fmt"
	ffmt "gopkg.in/ffmt.v1"
	"io/ioutil"
	"net/http"
	"net/url"
	"runtime/debug"
	"time"
)

type MetricType struct {
	NodeName string `json:"node_name"`
}

type ResultType struct {
	Metric MetricType    `json:"metric"`
	Value  []interface{} `json:"value"`
}

type QueryData struct {
	ResultType string       `json:"resultType"`
	Result     []ResultType `json:"result"`
}

type QueryInfo struct {
	Status string    `json:"status"`
	Data   QueryData `json:"data"`
}

func GetPromResult(url string, result interface{}) error {
	httpClient := &http.Client{Timeout: 10 * time.Second}
	r, err := httpClient.Get(url)
	if err != nil {
		return err
	}

	defer r.Body.Close()
	bt, err := ioutil.ReadAll(r.Body)
	if err != nil {
		return err
	}
	fmt.Println(string(bt))

	err = json.Unmarshal(bt, result)
	if err != nil {
		fmt.Printf("%s", debug.Stack())
		debug.PrintStack()
		return err
	}
	return nil
}

//QueryMetric query metric by prom api
func QueryMetric(endpoint string, query string) (*QueryInfo, error) {
	info := &QueryInfo{}
	ustr := endpoint + "/api/v1/query?query=" + query
	u, err := url.Parse(ustr)
	if err != nil {
		return info, err
	}
	u.RawQuery = u.Query().Encode()

	err = GetPromResult(u.String(), &info)
	if err != nil {
		ffmt.Puts(info)
		return info, err
	}
	return info, nil
}

//func QueryMetric2(endpoint string, query string) (prom_model.Value, error) {
//	client, err := api.NewClient(api.Config{
//		Address: endpoint,
//	})
//	if err != nil {
//		fmt.Printf("Error creating client: %v\n", err)
//		return nil, err
//	}
//
//	v1api := v1.NewAPI(client)
//	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
//	defer cancel()
//	result, warnings, err := v1api.Query(ctx, query, time.Now())
//	if err != nil {
//		fmt.Printf("Error querying Prometheus: %v\n", err)
//		return nil, err
//	}
//	if len(warnings) > 0 {
//		fmt.Printf("Warnings: %v\n", warnings)
//	}
//	fmt.Printf("Result:\n %v \n", result)
//	return result, nil
//}
