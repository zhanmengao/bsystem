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
	"net"
	"net/http"

	"github.com/gorilla/mux"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"

	"github.com/jaegertracing/jaeger/cmd/collector/app/handler"
	"github.com/jaegertracing/jaeger/cmd/collector/app/sampling/strategystore"
	clientcfgHandler "github.com/jaegertracing/jaeger/pkg/clientcfg/clientcfghttp"
	"github.com/jaegertracing/jaeger/pkg/config/tlscfg"
	"github.com/jaegertracing/jaeger/pkg/healthcheck"
	"github.com/jaegertracing/jaeger/pkg/httpmetrics"
	"github.com/jaegertracing/jaeger/pkg/metrics"
	"github.com/jaegertracing/jaeger/pkg/recoveryhandler"
)

// HTTPServerParams to construct a new Jaeger Collector HTTP Server
type HTTPServerParams struct {
	TLSConfig      tlscfg.Options
	HostPort       string
	Handler        handler.JaegerBatchesHandler
	SamplingStore  strategystore.StrategyStore
	MetricsFactory metrics.Factory
	HealthCheck    *healthcheck.HealthCheck
	Logger         *zap.Logger
}

// StartHTTPServer based on the given parameters
func StartHTTPServer(params *HTTPServerParams) (*http.Server, error) {
	params.Logger.Info("Starting jaeger-collector HTTP server", zap.String("http host-port", params.HostPort))

	errorLog, _ := zap.NewStdLogAt(params.Logger, zapcore.ErrorLevel)
	server := &http.Server{
		Addr:     params.HostPort,
		ErrorLog: errorLog,
	}
	if params.TLSConfig.Enabled {
		tlsCfg, err := params.TLSConfig.Config(params.Logger) // This checks if the certificates are correctly provided
		if err != nil {
			return nil, err
		}
		server.TLSConfig = tlsCfg
	}

	listener, err := net.Listen("tcp", params.HostPort)
	if err != nil {
		return nil, err
	}

	serveHTTP(server, listener, params)

	return server, nil
}

func serveHTTP(server *http.Server, listener net.Listener, params *HTTPServerParams) {
	r := mux.NewRouter()
	apiHandler := handler.NewAPIHandler(params.Handler)
	apiHandler.RegisterRoutes(r)

	cfgHandler := clientcfgHandler.NewHTTPHandler(clientcfgHandler.HTTPHandlerParams{
		ConfigManager: &clientcfgHandler.ConfigManager{
			SamplingStrategyStore: params.SamplingStore,
			// TODO provide baggage manager
		},
		MetricsFactory:         params.MetricsFactory,
		BasePath:               "/api",
		LegacySamplingEndpoint: false,
	})
	cfgHandler.RegisterRoutes(r)

	recoveryHandler := recoveryhandler.NewRecoveryHandler(params.Logger, true)
	server.Handler = httpmetrics.Wrap(recoveryHandler(r), params.MetricsFactory)
	go func() {
		var err error
		if params.TLSConfig.Enabled {
			err = server.ServeTLS(listener, "", "")
		} else {
			err = server.Serve(listener)
		}
		if err != nil {
			if err != http.ErrServerClosed {
				params.Logger.Error("Could not start HTTP collector", zap.Error(err))
			}
		}
		params.HealthCheck.Set(healthcheck.Unavailable)
	}()
}
