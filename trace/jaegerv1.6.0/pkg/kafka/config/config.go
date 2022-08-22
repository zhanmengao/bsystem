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

package config

import (
	"github.com/Shopify/sarama"
)

// Configuration describes the configuration properties needed to create a Kafka producer
type Configuration struct {
	Brokers []string
}

// ProducerBuilder builds a new kafka producer
type ProducerBuilder interface {
	NewProducer() (sarama.AsyncProducer, error)
}

// NewProducer creates a new asynchronous kafka producer
func (c *Configuration) NewProducer() (sarama.AsyncProducer, error) {
	saramaConfig := sarama.NewConfig()
	saramaConfig.Producer.Return.Successes = true
	return sarama.NewAsyncProducer(c.Brokers, saramaConfig)
}
