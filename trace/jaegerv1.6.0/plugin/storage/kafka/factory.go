// Copyright (c) 2018 The Jaeger Authors.
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

package kafka

import (
	"errors"
	"flag"

	"github.com/Shopify/sarama"
	"github.com/spf13/viper"
	"github.com/uber/jaeger-lib/metrics"
	"go.uber.org/zap"

	"github.com/jaegertracing/jaeger/pkg/kafka/config"
	"github.com/jaegertracing/jaeger/storage/dependencystore"
	"github.com/jaegertracing/jaeger/storage/spanstore"
)

// Factory implements storage.Factory and creates write-only storage components backed by kafka.
type Factory struct {
	options Options

	metricsFactory metrics.Factory
	logger         *zap.Logger

	config     config.ProducerBuilder
	producer   sarama.AsyncProducer
	marshaller Marshaller
}

// NewFactory creates a new Factory.
func NewFactory() *Factory {
	return &Factory{}
}

// AddFlags implements plugin.Configurable
func (f *Factory) AddFlags(flagSet *flag.FlagSet) {
	f.options.AddFlags(flagSet)
}

// InitFromViper implements plugin.Configurable
func (f *Factory) InitFromViper(v *viper.Viper) {
	f.options.InitFromViper(v)
	f.config = &f.options.config
}

// Initialize implements storage.Factory
func (f *Factory) Initialize(metricsFactory metrics.Factory, logger *zap.Logger) error {
	f.metricsFactory, f.logger = metricsFactory, logger
	logger.Info("Kafka storage configuration",
		zap.Any("producer config", f.config),
		zap.Any("topic", f.options.topic))
	p, err := f.config.NewProducer()
	if err != nil {
		return err
	}
	f.producer = p
	switch f.options.encoding {
	case encodingProto:
		f.marshaller = newProtobufMarshaller()
	case encodingJSON:
		f.marshaller = newJSONMarshaller()
	default:
		return errors.New("kafka encoding is not one of '" + encodingJSON + "' or '" + encodingProto + "'")
	}
	return nil
}

// CreateSpanReader implements storage.Factory
func (f *Factory) CreateSpanReader() (spanstore.Reader, error) {
	return nil, errors.New("kafka storage is write-only")
}

// CreateSpanWriter implements storage.Factory
func (f *Factory) CreateSpanWriter() (spanstore.Writer, error) {
	return NewSpanWriter(f.producer, f.marshaller, f.options.topic, f.metricsFactory), nil
}

// CreateDependencyReader implements storage.Factory
func (f *Factory) CreateDependencyReader() (dependencystore.Reader, error) {
	return nil, errors.New("kafka storage is write-only")
}
