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

/*
Package reducer implements a metrics Processor component to reduce attributes.

This package is currently in a pre-GA phase. Backwards incompatible changes
may be introduced in subsequent minor version releases as we work to track the
evolving OpenTelemetry specification and user feedback.

The metrics Processor component this package implements applies an
attribute.Filter to each processed export.Accumulation to remove attributes
before passing the result to another Processor.  This Processor can be used to
reduce inherent dimensionality in the data, as a way to control the cost of
collecting high cardinality metric data.

For example, to compose a push controller with a reducer and a basic
metric processor:

type someFilter struct{
        // configuration for this filter
        // ...
}

func (someFilter) AttributeFilterFor(_ *sdkapi.Descriptor) attribute.Filter {
        return func(attr kv.KeyValue) bool {
                // return true to keep this attr, false to drop this attr.
                // ...
        }
}

func setupMetrics(exporter export.Exporter) (stop func()) {
        basicProcessorFactory := basic.NewFactory(
                simple.NewWithHistogramDistribution(),
                exporter,
        )

        reducerProcessor := reducer.NewFactory(someFilter{...}, basicProcessorFactory)

        controller := controller.New(
                reducerProcessor,
                exporter,
                opts...,
        )
        controller.Start()
        global.SetMeterProvider(controller.Provider())
        return controller.Stop
*/
package reducer // import "go.opentelemetry.io/otel/sdk/metric/processor/reducer"
