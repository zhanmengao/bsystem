// Copyright (c) 2020 The Jaeger Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package flags

import (
	"crypto/tls"
	"fmt"
	"net"
	"net/http"
	"strconv"
	"strings"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"go.uber.org/zap"
	"go.uber.org/zap/zaptest/observer"

	"github.com/jaegertracing/jaeger/pkg/config"
	"github.com/jaegertracing/jaeger/pkg/config/tlscfg"
	"github.com/jaegertracing/jaeger/pkg/healthcheck"
	"github.com/jaegertracing/jaeger/ports"
)

var testCertKeyLocation = "../../pkg/config/tlscfg/testdata"

func TestAdminServerHandlesPortZero(t *testing.T) {
	adminServer := NewAdminServer(":0")

	v, _ := config.Viperize(adminServer.AddFlags)

	zapCore, logs := observer.New(zap.InfoLevel)
	logger := zap.New(zapCore)

	adminServer.initFromViper(v, logger)

	assert.NoError(t, adminServer.Serve())
	defer adminServer.Close()

	message := logs.FilterMessage("Admin server started")
	assert.Equal(t, 1, message.Len(), "Expected Admin server started log message.")

	onlyEntry := message.All()[0]
	hostPort := onlyEntry.ContextMap()["http.host-port"].(string)
	port, _ := strconv.Atoi(strings.Split(hostPort, ":")[3])
	assert.Greater(t, port, 0)
}

func TestAdminHealthCheck(t *testing.T) {
	adminServer := NewAdminServer(":0")
	status := adminServer.HC().Get()
	assert.Equal(t, healthcheck.Unavailable, status)
}

func TestAdminFailToServe(t *testing.T) {
	l, err := net.Listen("tcp", ":0")
	require.NoError(t, err)
	l.Close() // forcing Serve on a closed connection

	adminServer := NewAdminServer(":0")
	v, command := config.Viperize(adminServer.AddFlags)
	command.ParseFlags([]string{})
	zapCore, logs := observer.New(zap.InfoLevel)
	logger := zap.New(zapCore)

	require.NoError(t, adminServer.initFromViper(v, logger))

	adminServer.serveWithListener(l)
	defer adminServer.Close()

	waitForEqual(t, healthcheck.Broken, func() interface{} { return adminServer.HC().Get() })

	logEntries := logs.TakeAll()
	var matchedEntry string
	for _, log := range logEntries {
		if strings.Contains(log.Message, "failed to serve") {
			matchedEntry = log.Message
			break
		}
	}
	assert.Contains(t, matchedEntry, "failed to serve")
}

func TestAdminWithFailedFlags(t *testing.T) {
	adminServer := NewAdminServer(fmt.Sprintf(":%d", ports.CollectorAdminHTTP))
	zapCore, _ := observer.New(zap.InfoLevel)
	logger := zap.New(zapCore)
	v, command := config.Viperize(adminServer.AddFlags)
	err := command.ParseFlags([]string{
		"--admin.http.tls.enabled=false",
		"--admin.http.tls.cert=blah", // invalid unless tls.enabled
	})
	require.NoError(t, err)
	err = adminServer.initFromViper(v, logger)
	require.Error(t, err)
	assert.Contains(t, err.Error(), "failed to parse admin server TLS options")
}

func TestAdminServerTLS(t *testing.T) {
	testCases := []struct {
		name           string
		serverTLSFlags []string
		clientTLS      tlscfg.Options
	}{
		{
			name: "should pass with TLS client to trusted TLS server with correct hostname",
			serverTLSFlags: []string{
				"--admin.http.tls.enabled=true",
				"--admin.http.tls.cert=" + testCertKeyLocation + "/example-server-cert.pem",
				"--admin.http.tls.key=" + testCertKeyLocation + "/example-server-key.pem",
			},
			clientTLS: tlscfg.Options{
				Enabled:    true,
				CAPath:     testCertKeyLocation + "/example-CA-cert.pem",
				ServerName: "example.com",
			},
		},
	}

	for _, test := range testCases {
		t.Run(test.name, func(t *testing.T) {
			adminServer := NewAdminServer(fmt.Sprintf(":%d", ports.CollectorAdminHTTP))

			v, command := config.Viperize(adminServer.AddFlags)
			err := command.ParseFlags(test.serverTLSFlags)
			require.NoError(t, err)
			zapCore, _ := observer.New(zap.InfoLevel)
			logger := zap.New(zapCore)

			err = adminServer.initFromViper(v, logger)
			require.NoError(t, err)

			adminServer.Serve()
			defer adminServer.Close()

			clientTLSCfg, err0 := test.clientTLS.Config(zap.NewNop())
			require.NoError(t, err0)
			dialer := &net.Dialer{Timeout: 2 * time.Second}
			conn, clientError := tls.DialWithDialer(dialer, "tcp", fmt.Sprintf("localhost:%d", ports.CollectorAdminHTTP), clientTLSCfg)
			require.NoError(t, clientError)
			require.Nil(t, conn.Close())

			client := &http.Client{
				Transport: &http.Transport{
					TLSClientConfig: clientTLSCfg,
				},
			}

			response, requestError := client.Get(fmt.Sprintf("https://localhost:%d", ports.CollectorAdminHTTP))
			require.NoError(t, requestError)
			require.NotNil(t, response)
		})
	}
}
