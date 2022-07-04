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

package basic_test

import (
	"context"
	"runtime"
	"testing"
	"time"

	"github.com/stretchr/testify/require"

	"go.opentelemetry.io/otel/attribute"
	controller "go.opentelemetry.io/otel/sdk/metric/controller/basic"
	"go.opentelemetry.io/otel/sdk/metric/controller/controllertest"
	"go.opentelemetry.io/otel/sdk/metric/export/aggregation"
	processor "go.opentelemetry.io/otel/sdk/metric/processor/basic"
	"go.opentelemetry.io/otel/sdk/metric/processor/processortest"
	"go.opentelemetry.io/otel/sdk/resource"
)

func TestPullNoCollect(t *testing.T) {
	puller := controller.New(
		processor.NewFactory(
			processortest.AggregatorSelector(),
			aggregation.CumulativeTemporalitySelector(),
			processor.WithMemory(true),
		),
		controller.WithCollectPeriod(0),
		controller.WithResource(resource.Empty()),
	)

	ctx := context.Background()
	meter := puller.Meter("nocache")
	counter, err := meter.SyncInt64().Counter("counter.sum")
	require.NoError(t, err)

	counter.Add(ctx, 10, attribute.String("A", "B"))

	require.NoError(t, puller.Collect(ctx))
	records := processortest.NewOutput(attribute.DefaultEncoder())
	require.NoError(t, controllertest.ReadAll(puller, aggregation.CumulativeTemporalitySelector(), records.AddInstrumentationLibraryRecord))

	require.EqualValues(t, map[string]float64{
		"counter.sum/A=B/": 10,
	}, records.Map())

	counter.Add(ctx, 10, attribute.String("A", "B"))

	require.NoError(t, puller.Collect(ctx))
	records = processortest.NewOutput(attribute.DefaultEncoder())
	require.NoError(t, controllertest.ReadAll(puller, aggregation.CumulativeTemporalitySelector(), records.AddInstrumentationLibraryRecord))

	require.EqualValues(t, map[string]float64{
		"counter.sum/A=B/": 20,
	}, records.Map())
}

func TestPullWithCollect(t *testing.T) {
	puller := controller.New(
		processor.NewFactory(
			processortest.AggregatorSelector(),
			aggregation.CumulativeTemporalitySelector(),
			processor.WithMemory(true),
		),
		controller.WithCollectPeriod(time.Second),
		controller.WithResource(resource.Empty()),
	)
	mock := controllertest.NewMockClock()
	puller.SetClock(mock)

	ctx := context.Background()
	meter := puller.Meter("nocache")
	counter, err := meter.SyncInt64().Counter("counter.sum")
	require.NoError(t, err)

	counter.Add(ctx, 10, attribute.String("A", "B"))

	require.NoError(t, puller.Collect(ctx))
	records := processortest.NewOutput(attribute.DefaultEncoder())
	require.NoError(t, controllertest.ReadAll(puller, aggregation.CumulativeTemporalitySelector(), records.AddInstrumentationLibraryRecord))

	require.EqualValues(t, map[string]float64{
		"counter.sum/A=B/": 10,
	}, records.Map())

	counter.Add(ctx, 10, attribute.String("A", "B"))

	// Cached value!
	require.NoError(t, puller.Collect(ctx))
	records = processortest.NewOutput(attribute.DefaultEncoder())
	require.NoError(t, controllertest.ReadAll(puller, aggregation.CumulativeTemporalitySelector(), records.AddInstrumentationLibraryRecord))

	require.EqualValues(t, map[string]float64{
		"counter.sum/A=B/": 10,
	}, records.Map())

	mock.Add(time.Second)
	runtime.Gosched()

	// Re-computed value!
	require.NoError(t, puller.Collect(ctx))
	records = processortest.NewOutput(attribute.DefaultEncoder())
	require.NoError(t, controllertest.ReadAll(puller, aggregation.CumulativeTemporalitySelector(), records.AddInstrumentationLibraryRecord))

	require.EqualValues(t, map[string]float64{
		"counter.sum/A=B/": 20,
	}, records.Map())

}
