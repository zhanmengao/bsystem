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

package app

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"go.uber.org/zap"

	"github.com/jaegertracing/jaeger/cmd/collector/app/flags"
	cmdFlags "github.com/jaegertracing/jaeger/cmd/flags"
	"github.com/jaegertracing/jaeger/pkg/config"
	"github.com/jaegertracing/jaeger/pkg/metrics"
	"github.com/jaegertracing/jaeger/pkg/tenancy"
	"github.com/jaegertracing/jaeger/plugin/storage/memory"
)

func TestNewSpanHandlerBuilder(t *testing.T) {
	v, command := config.Viperize(cmdFlags.AddFlags, flags.AddFlags)

	require.NoError(t, command.ParseFlags([]string{}))
	cOpts, err := new(flags.CollectorOptions).InitFromViper(v, zap.NewNop())
	require.NoError(t, err)

	spanWriter := memory.NewStore()

	builder := &SpanHandlerBuilder{
		SpanWriter:    spanWriter,
		CollectorOpts: cOpts,
		TenancyMgr:    &tenancy.Manager{},
	}
	assert.NotNil(t, builder.logger())
	assert.NotNil(t, builder.metricsFactory())

	builder = &SpanHandlerBuilder{
		SpanWriter:     spanWriter,
		CollectorOpts:  cOpts,
		Logger:         zap.NewNop(),
		MetricsFactory: metrics.NullFactory,
		TenancyMgr:     &tenancy.Manager{},
	}

	spanProcessor := builder.BuildSpanProcessor()
	spanHandlers := builder.BuildHandlers(spanProcessor)
	assert.NotNil(t, spanHandlers.ZipkinSpansHandler)
	assert.NotNil(t, spanHandlers.JaegerBatchesHandler)
	assert.NotNil(t, spanHandlers.GRPCHandler)
	assert.NotNil(t, spanProcessor)
}

func TestDefaultSpanFilter(t *testing.T) {
	assert.True(t, defaultSpanFilter(nil))
}
