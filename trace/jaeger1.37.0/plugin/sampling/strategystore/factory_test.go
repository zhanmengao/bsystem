// Copyright (c) 2019 The Jaeger Authors.
// Copyright (c) 2018 Uber Technologies, Inc.
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

package strategystore

import (
	"errors"
	"flag"
	"os"
	"testing"

	"github.com/spf13/viper"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"go.uber.org/zap"

	ss "github.com/jaegertracing/jaeger/cmd/collector/app/sampling/strategystore"
	"github.com/jaegertracing/jaeger/pkg/distributedlock"
	"github.com/jaegertracing/jaeger/pkg/metrics"
	"github.com/jaegertracing/jaeger/plugin"
	"github.com/jaegertracing/jaeger/storage"
	"github.com/jaegertracing/jaeger/storage/samplingstore"
)

func clearEnv() {
	os.Setenv(SamplingTypeEnvVar, "static")
}

var (
	_ ss.Factory          = new(Factory)
	_ plugin.Configurable = new(Factory)
)

func TestNewFactory(t *testing.T) {
	tests := []struct {
		strategyStoreType string
		expectError       bool
	}{
		{
			strategyStoreType: "file",
		},
		{
			strategyStoreType: "adaptive",
		},
		{
			// expliclitly test that the deprecated value is refused in NewFactory(). it should be translated correctly in factory_config.go
			// and no other code should need to be aware of the old name.
			strategyStoreType: "static",
			expectError:       true,
		},
		{
			strategyStoreType: "nonsense",
			expectError:       true,
		},
	}

	mockSSFactory := &mockSamplingStoreFactory{}

	for _, tc := range tests {
		f, err := NewFactory(FactoryConfig{StrategyStoreType: Kind(tc.strategyStoreType)})
		if tc.expectError {
			assert.Error(t, err)
			continue
		}
		assert.NotEmpty(t, f.factories)
		assert.NotEmpty(t, f.factories[Kind(tc.strategyStoreType)])
		assert.Equal(t, Kind(tc.strategyStoreType), f.StrategyStoreType)

		mock := new(mockFactory)
		f.factories[Kind(tc.strategyStoreType)] = mock

		assert.NoError(t, f.Initialize(metrics.NullFactory, mockSSFactory, zap.NewNop()))
		_, _, err = f.CreateStrategyStore()
		require.NoError(t, err)

		// force the mock to return errors
		mock.retError = true
		assert.EqualError(t, f.Initialize(metrics.NullFactory, mockSSFactory, zap.NewNop()), "error initializing store")
		_, _, err = f.CreateStrategyStore()
		assert.EqualError(t, err, "error creating store")

		// request something that doesn't exist
		f.StrategyStoreType = "doesntexist"
		_, _, err = f.CreateStrategyStore()
		assert.EqualError(t, err, "no doesntexist strategy store registered")
	}
}

func TestConfigurable(t *testing.T) {
	clearEnv()
	defer clearEnv()

	f, err := NewFactory(FactoryConfig{StrategyStoreType: "file"})
	require.NoError(t, err)
	assert.NotEmpty(t, f.factories)
	assert.NotEmpty(t, f.factories["file"])

	mock := new(mockFactory)
	f.factories["file"] = mock

	fs := new(flag.FlagSet)
	v := viper.New()

	f.AddFlags(fs)
	f.InitFromViper(v, zap.NewNop())

	assert.Equal(t, fs, mock.flagSet)
	assert.Equal(t, v, mock.viper)
}

type mockFactory struct {
	flagSet  *flag.FlagSet
	viper    *viper.Viper
	logger   *zap.Logger
	retError bool
}

func (f *mockFactory) AddFlags(flagSet *flag.FlagSet) {
	f.flagSet = flagSet
}

func (f *mockFactory) InitFromViper(v *viper.Viper, logger *zap.Logger) {
	f.viper = v
	f.logger = logger
}

func (f *mockFactory) CreateStrategyStore() (ss.StrategyStore, ss.Aggregator, error) {
	if f.retError {
		return nil, nil, errors.New("error creating store")
	}
	return nil, nil, nil
}

func (f *mockFactory) Initialize(metricsFactory metrics.Factory, ssFactory storage.SamplingStoreFactory, logger *zap.Logger) error {
	if f.retError {
		return errors.New("error initializing store")
	}
	return nil
}

type mockSamplingStoreFactory struct{}

func (m *mockSamplingStoreFactory) CreateLock() (distributedlock.Lock, error) {
	return nil, nil
}

func (m *mockSamplingStoreFactory) CreateSamplingStore(maxBuckets int) (samplingstore.Store, error) {
	return nil, nil
}
