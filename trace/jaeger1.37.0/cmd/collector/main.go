// Copyright (c) 2019 The Jaeger Authors.
// Copyright (c) 2017 Uber Technologies, Inc.
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

package main

import (
	"fmt"
	"io"
	"log"
	"os"

	"github.com/spf13/cobra"
	"github.com/spf13/viper"
	_ "go.uber.org/automaxprocs"
	"go.uber.org/zap"

	"github.com/jaegertracing/jaeger/cmd/collector/app"
	"github.com/jaegertracing/jaeger/cmd/collector/app/flags"
	"github.com/jaegertracing/jaeger/cmd/docs"
	"github.com/jaegertracing/jaeger/cmd/env"
	cmdFlags "github.com/jaegertracing/jaeger/cmd/flags"
	"github.com/jaegertracing/jaeger/cmd/status"
	"github.com/jaegertracing/jaeger/internal/metrics/expvar"
	"github.com/jaegertracing/jaeger/internal/metrics/fork"
	"github.com/jaegertracing/jaeger/pkg/config"
	"github.com/jaegertracing/jaeger/pkg/metrics"
	"github.com/jaegertracing/jaeger/pkg/tenancy"
	"github.com/jaegertracing/jaeger/pkg/version"
	ss "github.com/jaegertracing/jaeger/plugin/sampling/strategystore"
	"github.com/jaegertracing/jaeger/plugin/storage"
	"github.com/jaegertracing/jaeger/ports"
)

const serviceName = "jaeger-collector"

func main() {
	svc := cmdFlags.NewService(ports.CollectorAdminHTTP)

	storageFactory, err := storage.NewFactory(storage.FactoryConfigFromEnvAndCLI(os.Args, os.Stderr))
	if err != nil {
		log.Fatalf("Cannot initialize storage factory: %v", err)
	}
	strategyStoreFactoryConfig, err := ss.FactoryConfigFromEnv(os.Stderr)
	if err != nil {
		log.Fatalf("Cannot initialize sampling strategy store factory config: %v", err)
	}
	strategyStoreFactory, err := ss.NewFactory(*strategyStoreFactoryConfig)
	if err != nil {
		log.Fatalf("Cannot initialize sampling strategy store factory: %v", err)
	}

	v := viper.New()
	command := &cobra.Command{
		Use:   "jaeger-collector",
		Short: "Jaeger collector receives and processes traces from Jaeger agents and clients",
		Long:  `Jaeger collector receives traces from Jaeger agents and runs them through a processing pipeline.`,
		RunE: func(cmd *cobra.Command, args []string) error {
			if err := svc.Start(v); err != nil {
				return err
			}
			logger := svc.Logger // shortcut
			baseFactory := svc.MetricsFactory.Namespace(metrics.NSOptions{Name: "jaeger"})
			metricsFactory := fork.New("internal",
				expvar.NewFactory(10), // backend for internal opts
				baseFactory.Namespace(metrics.NSOptions{Name: "collector"}))
			version.NewInfoMetrics(metricsFactory)

			storageFactory.InitFromViper(v, logger)
			if err := storageFactory.Initialize(baseFactory, logger); err != nil {
				logger.Fatal("Failed to init storage factory", zap.Error(err))
			}
			spanWriter, err := storageFactory.CreateSpanWriter()
			if err != nil {
				logger.Fatal("Failed to create span writer", zap.Error(err))
			}

			ssFactory, err := storageFactory.CreateSamplingStoreFactory()
			if err != nil {
				logger.Fatal("Failed to create sampling store factory", zap.Error(err))
			}

			strategyStoreFactory.InitFromViper(v, logger)
			if err := strategyStoreFactory.Initialize(metricsFactory, ssFactory, logger); err != nil {
				logger.Fatal("Failed to init sampling strategy store factory", zap.Error(err))
			}
			strategyStore, aggregator, err := strategyStoreFactory.CreateStrategyStore()
			if err != nil {
				logger.Fatal("Failed to create sampling strategy store", zap.Error(err))
			}
			collectorOpts, err := new(flags.CollectorOptions).InitFromViper(v, logger)
			if err != nil {
				logger.Fatal("Failed to initialize collector", zap.Error(err))
			}
			tm := tenancy.NewManager(&collectorOpts.GRPC.Tenancy)

			collector := app.New(&app.CollectorParams{
				ServiceName:    serviceName,
				Logger:         logger,
				MetricsFactory: metricsFactory,
				SpanWriter:     spanWriter,
				StrategyStore:  strategyStore,
				Aggregator:     aggregator,
				HealthCheck:    svc.HC(),
				TenancyMgr:     tm,
			})
			// Start all Collector services
			if err := collector.Start(collectorOpts); err != nil {
				logger.Fatal("Failed to start collector", zap.Error(err))
			}
			// Wait for shutfown
			svc.RunAndThen(func() {
				if err := collector.Close(); err != nil {
					logger.Error("failed to cleanly close the collector", zap.Error(err))
				}
				if closer, ok := spanWriter.(io.Closer); ok {
					err := closer.Close()
					if err != nil {
						logger.Error("failed to close span writer", zap.Error(err))
					}
				}
				if err := storageFactory.Close(); err != nil {
					logger.Error("Failed to close storage factory", zap.Error(err))
				}
			})
			return nil
		},
	}

	command.AddCommand(version.Command())
	command.AddCommand(env.Command())
	command.AddCommand(docs.Command(v))
	command.AddCommand(status.Command(v, ports.CollectorAdminHTTP))

	config.AddFlags(
		v,
		command,
		svc.AddFlags,
		flags.AddFlags,
		storageFactory.AddPipelineFlags,
		strategyStoreFactory.AddFlags,
	)

	if err := command.Execute(); err != nil {
		fmt.Println(err.Error())
		os.Exit(1)
	}
}
