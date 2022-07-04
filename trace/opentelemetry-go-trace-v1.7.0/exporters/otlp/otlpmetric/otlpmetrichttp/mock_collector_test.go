// Copyright The OpenTelemetry Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package otlpmetrichttp_test

import (
	"bytes"
	"compress/gzip"
	"context"
	"crypto/tls"
	"fmt"
	"io"
	"io/ioutil"
	"net"
	"net/http"
	"sync"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"google.golang.org/protobuf/proto"

	"go.opentelemetry.io/otel/exporters/otlp/otlpmetric/internal/otlpconfig"
	"go.opentelemetry.io/otel/exporters/otlp/otlpmetric/internal/otlpmetrictest"
	collectormetricpb "go.opentelemetry.io/proto/otlp/collector/metrics/v1"
	metricpb "go.opentelemetry.io/proto/otlp/metrics/v1"
)

type mockCollector struct {
	endpoint string
	server   *http.Server

	spanLock       sync.Mutex
	metricsStorage otlpmetrictest.MetricsStorage

	injectHTTPStatus  []int
	injectContentType string
	delay             <-chan struct{}

	clientTLSConfig *tls.Config
	expectedHeaders map[string]string
}

func (c *mockCollector) Stop() error {
	return c.server.Shutdown(context.Background())
}

func (c *mockCollector) MustStop(t *testing.T) {
	assert.NoError(t, c.server.Shutdown(context.Background()))
}

func (c *mockCollector) GetMetrics() []*metricpb.Metric {
	c.spanLock.Lock()
	defer c.spanLock.Unlock()
	return c.metricsStorage.GetMetrics()
}

func (c *mockCollector) Endpoint() string {
	return c.endpoint
}

func (c *mockCollector) ClientTLSConfig() *tls.Config {
	return c.clientTLSConfig
}

func (c *mockCollector) serveMetrics(w http.ResponseWriter, r *http.Request) {
	if c.delay != nil {
		select {
		case <-c.delay:
		case <-r.Context().Done():
			return
		}
	}

	if !c.checkHeaders(r) {
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	response := collectormetricpb.ExportMetricsServiceResponse{}
	rawResponse, err := proto.Marshal(&response)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		return
	}
	if injectedStatus := c.getInjectHTTPStatus(); injectedStatus != 0 {
		writeReply(w, rawResponse, injectedStatus, c.injectContentType)
		return
	}
	rawRequest, err := readRequest(r)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		return
	}

	request, err := unmarshalMetricsRequest(rawRequest, r.Header.Get("content-type"))
	if err != nil {
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	writeReply(w, rawResponse, 0, c.injectContentType)
	c.spanLock.Lock()
	defer c.spanLock.Unlock()
	c.metricsStorage.AddMetrics(request)
}

func unmarshalMetricsRequest(rawRequest []byte, contentType string) (*collectormetricpb.ExportMetricsServiceRequest, error) {
	request := &collectormetricpb.ExportMetricsServiceRequest{}
	if contentType != "application/x-protobuf" {
		return request, fmt.Errorf("invalid content-type: %s, only application/x-protobuf is supported", contentType)
	}
	err := proto.Unmarshal(rawRequest, request)
	return request, err
}

func (c *mockCollector) checkHeaders(r *http.Request) bool {
	for k, v := range c.expectedHeaders {
		got := r.Header.Get(k)
		if got != v {
			return false
		}
	}
	return true
}

func (c *mockCollector) getInjectHTTPStatus() int {
	if len(c.injectHTTPStatus) == 0 {
		return 0
	}
	status := c.injectHTTPStatus[0]
	c.injectHTTPStatus = c.injectHTTPStatus[1:]
	if len(c.injectHTTPStatus) == 0 {
		c.injectHTTPStatus = nil
	}
	return status
}

func readRequest(r *http.Request) ([]byte, error) {
	if r.Header.Get("Content-Encoding") == "gzip" {
		return readGzipBody(r.Body)
	}
	return ioutil.ReadAll(r.Body)
}

func readGzipBody(body io.Reader) ([]byte, error) {
	rawRequest := bytes.Buffer{}
	gunzipper, err := gzip.NewReader(body)
	if err != nil {
		return nil, err
	}
	defer gunzipper.Close()
	_, err = io.Copy(&rawRequest, gunzipper)
	if err != nil {
		return nil, err
	}
	return rawRequest.Bytes(), nil
}

func writeReply(w http.ResponseWriter, rawResponse []byte, injectHTTPStatus int, injectContentType string) {
	status := http.StatusOK
	if injectHTTPStatus != 0 {
		status = injectHTTPStatus
	}
	contentType := "application/x-protobuf"
	if injectContentType != "" {
		contentType = injectContentType
	}
	w.Header().Set("Content-Type", contentType)
	w.WriteHeader(status)
	_, _ = w.Write(rawResponse)
}

type mockCollectorConfig struct {
	MetricsURLPath    string
	Port              int
	InjectHTTPStatus  []int
	InjectContentType string
	Delay             <-chan struct{}
	WithTLS           bool
	ExpectedHeaders   map[string]string
}

func (c *mockCollectorConfig) fillInDefaults() {
	if c.MetricsURLPath == "" {
		c.MetricsURLPath = otlpconfig.DefaultMetricsPath
	}
}

func runMockCollector(t *testing.T, cfg mockCollectorConfig) *mockCollector {
	cfg.fillInDefaults()
	ln, err := net.Listen("tcp", fmt.Sprintf("localhost:%d", cfg.Port))
	require.NoError(t, err)
	_, portStr, err := net.SplitHostPort(ln.Addr().String())
	require.NoError(t, err)
	m := &mockCollector{
		endpoint:          fmt.Sprintf("localhost:%s", portStr),
		metricsStorage:    otlpmetrictest.NewMetricsStorage(),
		injectHTTPStatus:  cfg.InjectHTTPStatus,
		injectContentType: cfg.InjectContentType,
		delay:             cfg.Delay,
		expectedHeaders:   cfg.ExpectedHeaders,
	}
	mux := http.NewServeMux()
	mux.Handle(cfg.MetricsURLPath, http.HandlerFunc(m.serveMetrics))
	server := &http.Server{
		Handler: mux,
	}
	if cfg.WithTLS {
		pem, err := generateWeakCertificate()
		require.NoError(t, err)
		tlsCertificate, err := tls.X509KeyPair(pem.Certificate, pem.PrivateKey)
		require.NoError(t, err)
		server.TLSConfig = &tls.Config{
			Certificates: []tls.Certificate{tlsCertificate},
		}

		m.clientTLSConfig = &tls.Config{
			InsecureSkipVerify: true,
		}
	}
	go func() {
		if cfg.WithTLS {
			_ = server.ServeTLS(ln, "", "")
		} else {
			_ = server.Serve(ln)
		}
	}()
	m.server = server
	return m
}
