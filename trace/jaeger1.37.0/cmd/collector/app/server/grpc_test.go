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

package server

import (
	"context"
	"sync"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"go.uber.org/zap/zaptest/observer"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/grpc/test/bufconn"

	"github.com/jaegertracing/jaeger/cmd/collector/app/handler"
	"github.com/jaegertracing/jaeger/internal/grpctest"
	"github.com/jaegertracing/jaeger/pkg/config/tlscfg"
	"github.com/jaegertracing/jaeger/pkg/tenancy"
	"github.com/jaegertracing/jaeger/proto-gen/api_v2"
)

// test wrong port number
func TestFailToListen(t *testing.T) {
	logger, _ := zap.NewDevelopment()
	server, err := StartGRPCServer(&GRPCServerParams{
		HostPort:      ":-1",
		Handler:       handler.NewGRPCHandler(logger, &mockSpanProcessor{}, &tenancy.Manager{}),
		SamplingStore: &mockSamplingStore{},
		Logger:        logger,
	})
	assert.Nil(t, server)
	assert.EqualError(t, err, "failed to listen on gRPC port: listen tcp: address -1: invalid port")
}

func TestFailServe(t *testing.T) {
	lis := bufconn.Listen(0)
	lis.Close()
	core, logs := observer.New(zap.NewAtomicLevelAt(zapcore.ErrorLevel))
	var wg sync.WaitGroup
	wg.Add(1)

	logger := zap.New(core)
	serveGRPC(grpc.NewServer(), lis, &GRPCServerParams{
		Handler:       handler.NewGRPCHandler(logger, &mockSpanProcessor{}, &tenancy.Manager{}),
		SamplingStore: &mockSamplingStore{},
		Logger:        logger,
		OnError: func(e error) {
			assert.Equal(t, 1, len(logs.All()))
			assert.Equal(t, "Could not launch gRPC service", logs.All()[0].Message)
			wg.Done()
		},
	})
	wg.Wait()
}

func TestSpanCollector(t *testing.T) {
	logger, _ := zap.NewDevelopment()
	params := &GRPCServerParams{
		Handler:                 handler.NewGRPCHandler(logger, &mockSpanProcessor{}, &tenancy.Manager{}),
		SamplingStore:           &mockSamplingStore{},
		Logger:                  logger,
		MaxReceiveMessageLength: 1024 * 1024,
	}

	server, err := StartGRPCServer(params)
	require.NoError(t, err)
	defer server.Stop()

	conn, err := grpc.Dial(
		params.HostPortActual,
		grpc.WithTransportCredentials(insecure.NewCredentials()))
	require.NoError(t, err)
	defer conn.Close()

	c := api_v2.NewCollectorServiceClient(conn)
	response, err := c.PostSpans(context.Background(), &api_v2.PostSpansRequest{})
	require.NoError(t, err)
	require.NotNil(t, response)
}

func TestCollectorStartWithTLS(t *testing.T) {
	logger, _ := zap.NewDevelopment()
	params := &GRPCServerParams{
		Handler:       handler.NewGRPCHandler(logger, &mockSpanProcessor{}, &tenancy.Manager{}),
		SamplingStore: &mockSamplingStore{},
		Logger:        logger,
		TLSConfig: tlscfg.Options{
			Enabled:      true,
			CertPath:     testCertKeyLocation + "/example-server-cert.pem",
			KeyPath:      testCertKeyLocation + "/example-server-key.pem",
			ClientCAPath: testCertKeyLocation + "/example-CA-cert.pem",
		},
	}

	server, err := StartGRPCServer(params)
	require.NoError(t, err)
	defer server.Stop()
}

func TestCollectorReflection(t *testing.T) {
	logger, _ := zap.NewDevelopment()
	params := &GRPCServerParams{
		Handler:       handler.NewGRPCHandler(logger, &mockSpanProcessor{}, &tenancy.Manager{}),
		SamplingStore: &mockSamplingStore{},
		Logger:        logger,
	}

	server, err := StartGRPCServer(params)
	require.NoError(t, err)
	defer server.Stop()

	grpctest.ReflectionServiceValidator{
		HostPort: params.HostPortActual,
		Server:   server,
		ExpectedServices: []string{
			"jaeger.api_v2.CollectorService",
			"jaeger.api_v2.SamplingManager",
			"grpc.health.v1.Health",
		},
	}.Execute(t)
}
