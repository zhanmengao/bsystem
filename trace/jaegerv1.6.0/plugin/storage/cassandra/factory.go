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

package cassandra

import (
	"flag"

	"github.com/spf13/viper"
	"github.com/uber/jaeger-lib/metrics"
	"go.uber.org/zap"

	"github.com/jaegertracing/jaeger/pkg/cassandra"
	"github.com/jaegertracing/jaeger/pkg/cassandra/config"
	cDepStore "github.com/jaegertracing/jaeger/plugin/storage/cassandra/dependencystore"
	cSpanStore "github.com/jaegertracing/jaeger/plugin/storage/cassandra/spanstore"
	"github.com/jaegertracing/jaeger/storage"
	"github.com/jaegertracing/jaeger/storage/dependencystore"
	"github.com/jaegertracing/jaeger/storage/spanstore"
)

const (
	primaryStorageConfig = "cassandra"
	archiveStorageConfig = "cassandra-archive"
)

// Factory implements storage.Factory for Cassandra backend.
type Factory struct {
	Options *Options

	primaryMetricsFactory metrics.Factory
	archiveMetricsFactory metrics.Factory
	logger                *zap.Logger

	primaryConfig  config.SessionBuilder
	primarySession cassandra.Session
	archiveConfig  config.SessionBuilder
	archiveSession cassandra.Session
}

// NewFactory creates a new Factory.
func NewFactory() *Factory {
	return &Factory{
		Options: NewOptions(primaryStorageConfig, archiveStorageConfig),
	}
}

// AddFlags implements plugin.Configurable
func (f *Factory) AddFlags(flagSet *flag.FlagSet) {
	f.Options.AddFlags(flagSet)
}

// InitFromViper implements plugin.Configurable
func (f *Factory) InitFromViper(v *viper.Viper) {
	f.Options.InitFromViper(v)
	f.primaryConfig = f.Options.GetPrimary()
	if cfg := f.Options.Get(archiveStorageConfig); cfg != nil {
		f.archiveConfig = cfg // this is so stupid - see https://golang.org/doc/faq#nil_error
	}
}

// Initialize implements storage.Factory
func (f *Factory) Initialize(metricsFactory metrics.Factory, logger *zap.Logger) error {
	f.primaryMetricsFactory = metricsFactory.Namespace("cassandra", nil)
	f.archiveMetricsFactory = metricsFactory.Namespace("cassandra-archive", nil)
	f.logger = logger

	primarySession, err := f.primaryConfig.NewSession()
	if err != nil {
		return err
	}
	f.primarySession = primarySession

	if f.archiveConfig != nil {
		if archiveSession, err := f.archiveConfig.NewSession(); err == nil {
			f.archiveSession = archiveSession
		} else {
			return err
		}
	} else {
		logger.Info("Cassandra archive storage configuration is empty, skipping")
	}
	return nil
}

// CreateSpanReader implements storage.Factory
func (f *Factory) CreateSpanReader() (spanstore.Reader, error) {
	return cSpanStore.NewSpanReader(f.primarySession, f.primaryMetricsFactory, f.logger), nil
}

// CreateSpanWriter implements storage.Factory
func (f *Factory) CreateSpanWriter() (spanstore.Writer, error) {
	return cSpanStore.NewSpanWriter(f.primarySession, f.Options.SpanStoreWriteCacheTTL, f.primaryMetricsFactory, f.logger), nil
}

// CreateDependencyReader implements storage.Factory
func (f *Factory) CreateDependencyReader() (dependencystore.Reader, error) {
	return cDepStore.NewDependencyStore(f.primarySession, f.primaryMetricsFactory, f.logger), nil
}

// CreateArchiveSpanReader implements storage.ArchiveFactory
func (f *Factory) CreateArchiveSpanReader() (spanstore.Reader, error) {
	if f.archiveSession == nil {
		return nil, storage.ErrArchiveStorageNotConfigured
	}
	return cSpanStore.NewSpanReader(f.archiveSession, f.archiveMetricsFactory, f.logger), nil
}

// CreateArchiveSpanWriter implements storage.ArchiveFactory
func (f *Factory) CreateArchiveSpanWriter() (spanstore.Writer, error) {
	if f.archiveSession == nil {
		return nil, storage.ErrArchiveStorageNotConfigured
	}
	return cSpanStore.NewSpanWriter(f.archiveSession, f.Options.SpanStoreWriteCacheTTL, f.archiveMetricsFactory, f.logger), nil
}
