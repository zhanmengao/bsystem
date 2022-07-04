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

package opencensus

import (
	"context"
	"errors"
	"fmt"
	"testing"
	"time"

	"go.opencensus.io/metric/metricdata"
	ocresource "go.opencensus.io/resource"

	"go.opentelemetry.io/otel"
	"go.opentelemetry.io/otel/attribute"
	"go.opentelemetry.io/otel/metric/instrument"
	"go.opentelemetry.io/otel/metric/unit"
	"go.opentelemetry.io/otel/sdk/instrumentation"
	"go.opentelemetry.io/otel/sdk/metric/controller/controllertest"
	"go.opentelemetry.io/otel/sdk/metric/export"
	"go.opentelemetry.io/otel/sdk/metric/export/aggregation"
	"go.opentelemetry.io/otel/sdk/metric/metrictest"
	"go.opentelemetry.io/otel/sdk/metric/number"
	"go.opentelemetry.io/otel/sdk/metric/sdkapi"
	"go.opentelemetry.io/otel/sdk/resource"
)

type fakeExporter struct {
	export.Exporter
	records  []export.Record
	resource *resource.Resource
	err      error
}

func (f *fakeExporter) Export(ctx context.Context, res *resource.Resource, ilr export.InstrumentationLibraryReader) error {
	return controllertest.ReadAll(ilr, aggregation.StatelessTemporalitySelector(),
		func(_ instrumentation.Library, record export.Record) error {
			f.resource = res
			f.records = append(f.records, record)
			return f.err
		})
}

type fakeErrorHandler struct {
	err error
}

func (f *fakeErrorHandler) Handle(err error) {
	f.err = err
}

func (f *fakeErrorHandler) matches(err error) error {
	// make sure err is cleared for the next test
	defer func() { f.err = nil }()
	if !errors.Is(f.err, err) {
		return fmt.Errorf("err(%v), want err(%v)", f.err, err)
	}
	return nil
}

func TestExportMetrics(t *testing.T) {
	now := time.Now()
	basicDesc := metrictest.NewDescriptor(
		"",
		sdkapi.GaugeObserverInstrumentKind,
		number.Int64Kind,
	)
	fakeErrorHandler := &fakeErrorHandler{}
	otel.SetErrorHandler(fakeErrorHandler)
	for _, tc := range []struct {
		desc                 string
		input                []*metricdata.Metric
		exportErr            error
		expected             []export.Record
		expectedResource     *resource.Resource
		expectedHandledError error
	}{
		{
			desc: "no metrics",
		},
		{
			desc: "metric without points is dropped",
			input: []*metricdata.Metric{
				{
					TimeSeries: []*metricdata.TimeSeries{
						{},
					},
				},
			},
		},
		{
			desc: "descriptor conversion error",
			input: []*metricdata.Metric{
				// TypeGaugeDistribution isn't supported
				{Descriptor: metricdata.Descriptor{Type: metricdata.TypeGaugeDistribution}},
			},
			expectedHandledError: errConversion,
		},
		{
			desc: "attrs conversion error",
			input: []*metricdata.Metric{
				{
					// No descriptor with attribute keys.
					TimeSeries: []*metricdata.TimeSeries{
						// 1 attribute value, which doens't exist in keys.
						{
							LabelValues: []metricdata.LabelValue{{Value: "foo", Present: true}},
							Points: []metricdata.Point{
								{},
							},
						},
					},
				},
			},
			expectedHandledError: errConversion,
		},
		{
			desc: "unsupported summary point type",
			input: []*metricdata.Metric{
				{
					TimeSeries: []*metricdata.TimeSeries{
						{
							Points: []metricdata.Point{
								{Value: &metricdata.Summary{}},
							},
						},
					},
				},
			},
			exportErr: errIncompatibleType,
		},
		{
			desc: "success",
			input: []*metricdata.Metric{
				{
					Resource: &ocresource.Resource{
						Labels: map[string]string{
							"R1": "V1",
							"R2": "V2",
						},
					},
					TimeSeries: []*metricdata.TimeSeries{
						{
							StartTime: now,
							Points: []metricdata.Point{
								{Value: int64(123), Time: now},
							},
						},
					},
				},
			},
			expectedResource: resource.NewSchemaless(
				attribute.String("R1", "V1"),
				attribute.String("R2", "V2"),
			),
			expected: []export.Record{
				export.NewRecord(
					&basicDesc,
					attribute.EmptySet(),
					&ocRawAggregator{
						value: number.NewInt64Number(123),
						time:  now,
					},
					now,
					now,
				),
			},
		},
		{
			desc: "export error after success",
			input: []*metricdata.Metric{
				{
					TimeSeries: []*metricdata.TimeSeries{
						{
							StartTime: now,
							Points: []metricdata.Point{
								{Value: int64(123), Time: now},
							},
						},
					},
				},
			},
			expected: []export.Record{
				export.NewRecord(
					&basicDesc,
					attribute.EmptySet(),
					&ocRawAggregator{
						value: number.NewInt64Number(123),
						time:  now,
					},
					now,
					now,
				),
			},
			exportErr: errors.New("failed to export"),
		},
		{
			desc: "partial success sends correct metrics and drops incorrect metrics with handled err",
			input: []*metricdata.Metric{
				{
					TimeSeries: []*metricdata.TimeSeries{
						{
							StartTime: now,
							Points: []metricdata.Point{
								{Value: int64(123), Time: now},
							},
						},
					},
				},
				// TypeGaugeDistribution isn't supported
				{Descriptor: metricdata.Descriptor{Type: metricdata.TypeGaugeDistribution}},
			},
			expected: []export.Record{
				export.NewRecord(
					&basicDesc,
					attribute.EmptySet(),
					&ocRawAggregator{
						value: number.NewInt64Number(123),
						time:  now,
					},
					now,
					now,
				),
			},
			expectedHandledError: errConversion,
		},
	} {
		t.Run(tc.desc, func(t *testing.T) {
			fakeExporter := &fakeExporter{err: tc.exportErr}
			err := NewMetricExporter(fakeExporter).ExportMetrics(context.Background(), tc.input)
			if !errors.Is(err, tc.exportErr) {
				t.Errorf("NewMetricExporter(%+v) = err(%v), want err(%v)", tc.input, err, tc.exportErr)
			}
			// Check the global error handler, since we don't return errors
			// which occur during conversion.
			err = fakeErrorHandler.matches(tc.expectedHandledError)
			if err != nil {
				t.Fatalf("ExportMetrics(%+v) = %v", tc.input, err)
			}
			output := fakeExporter.records
			if len(tc.expected) != len(output) {
				t.Fatalf("ExportMetrics(%+v) = %d records, want %d records", tc.input, len(output), len(tc.expected))
			}
			if fakeExporter.resource.String() != tc.expectedResource.String() {
				t.Errorf("ExportMetrics(%+v)[i].Resource() = %+v, want %+v", tc.input, fakeExporter.resource.String(), tc.expectedResource.String())
			}
			for i, expected := range tc.expected {
				if output[i].StartTime() != expected.StartTime() {
					t.Errorf("ExportMetrics(%+v)[i].StartTime() = %+v, want %+v", tc.input, output[i].StartTime(), expected.StartTime())
				}
				if output[i].EndTime() != expected.EndTime() {
					t.Errorf("ExportMetrics(%+v)[i].EndTime() = %+v, want %+v", tc.input, output[i].EndTime(), expected.EndTime())
				}
				if output[i].Descriptor().Name() != expected.Descriptor().Name() {
					t.Errorf("ExportMetrics(%+v)[i].Descriptor() = %+v, want %+v", tc.input, output[i].Descriptor().Name(), expected.Descriptor().Name())
				}
				// Don't bother with a complete check of the descriptor.
				// That is checked as part of descriptor conversion tests below.
				if !output[i].Attributes().Equals(expected.Attributes()) {
					t.Errorf("ExportMetrics(%+v)[i].Attributes() = %+v, want %+v", tc.input, output[i].Attributes(), expected.Attributes())
				}
				if output[i].Aggregation().Kind() != expected.Aggregation().Kind() {
					t.Errorf("ExportMetrics(%+v)[i].Aggregation() = %+v, want %+v", tc.input, output[i].Aggregation().Kind(), expected.Aggregation().Kind())
				}
				// Don't bother checking the contents of the points aggregation.
				// Those tests are done with the aggregations themselves
			}
		})
	}
}

func TestConvertAttributes(t *testing.T) {
	setWithMultipleKeys := attribute.NewSet(
		attribute.KeyValue{Key: attribute.Key("first"), Value: attribute.StringValue("1")},
		attribute.KeyValue{Key: attribute.Key("second"), Value: attribute.StringValue("2")},
	)
	for _, tc := range []struct {
		desc        string
		inputKeys   []metricdata.LabelKey
		inputValues []metricdata.LabelValue
		expected    *attribute.Set
		expectedErr error
	}{
		{
			desc:     "no attributes",
			expected: attribute.EmptySet(),
		},
		{
			desc:        "different numbers of keys and values",
			inputKeys:   []metricdata.LabelKey{{Key: "foo"}},
			expected:    attribute.EmptySet(),
			expectedErr: errConversion,
		},
		{
			desc:      "multiple keys and values",
			inputKeys: []metricdata.LabelKey{{Key: "first"}, {Key: "second"}},
			inputValues: []metricdata.LabelValue{
				{Value: "1", Present: true},
				{Value: "2", Present: true},
			},
			expected: &setWithMultipleKeys,
		},
		{
			desc:      "multiple keys and values with some not present",
			inputKeys: []metricdata.LabelKey{{Key: "first"}, {Key: "second"}, {Key: "third"}},
			inputValues: []metricdata.LabelValue{
				{Value: "1", Present: true},
				{Value: "2", Present: true},
				{Present: false},
			},
			expected: &setWithMultipleKeys,
		},
	} {
		t.Run(tc.desc, func(t *testing.T) {
			output, err := convertAttrs(tc.inputKeys, tc.inputValues)
			if !errors.Is(err, tc.expectedErr) {
				t.Errorf("convertAttrs(keys: %v, values: %v) = err(%v), want err(%v)", tc.inputKeys, tc.inputValues, err, tc.expectedErr)
			}
			if !output.Equals(tc.expected) {
				t.Errorf("convertAttrs(keys: %v, values: %v) = %+v, want %+v", tc.inputKeys, tc.inputValues, output.ToSlice(), tc.expected.ToSlice())
			}
		})
	}
}
func TestConvertResource(t *testing.T) {
	for _, tc := range []struct {
		desc     string
		input    *ocresource.Resource
		expected *resource.Resource
	}{
		{
			desc: "nil resource",
		},
		{
			desc: "empty resource",
			input: &ocresource.Resource{
				Labels: map[string]string{},
			},
			expected: resource.NewSchemaless(),
		},
		{
			desc: "resource with attributes",
			input: &ocresource.Resource{
				Labels: map[string]string{
					"foo":  "bar",
					"tick": "tock",
				},
			},
			expected: resource.NewSchemaless(
				attribute.KeyValue{Key: attribute.Key("foo"), Value: attribute.StringValue("bar")},
				attribute.KeyValue{Key: attribute.Key("tick"), Value: attribute.StringValue("tock")},
			),
		},
	} {
		t.Run(tc.desc, func(t *testing.T) {
			output := convertResource(tc.input)
			if !output.Equal(tc.expected) {
				t.Errorf("convertResource(%v) = %+v, want %+v", tc.input, output, tc.expected)
			}
		})
	}
}
func TestConvertDescriptor(t *testing.T) {
	for _, tc := range []struct {
		desc        string
		input       metricdata.Descriptor
		expected    sdkapi.Descriptor
		expectedErr error
	}{
		{
			desc: "empty descriptor",
			expected: metrictest.NewDescriptor(
				"",
				sdkapi.GaugeObserverInstrumentKind,
				number.Int64Kind,
			),
		},
		{
			desc: "gauge int64 bytes",
			input: metricdata.Descriptor{
				Name:        "foo",
				Description: "bar",
				Unit:        metricdata.UnitBytes,
				Type:        metricdata.TypeGaugeInt64,
			},
			expected: metrictest.NewDescriptor(
				"foo",
				sdkapi.GaugeObserverInstrumentKind,
				number.Int64Kind,
				instrument.WithDescription("bar"),
				instrument.WithUnit(unit.Bytes),
			),
		},
		{
			desc: "gauge float64 ms",
			input: metricdata.Descriptor{
				Name:        "foo",
				Description: "bar",
				Unit:        metricdata.UnitMilliseconds,
				Type:        metricdata.TypeGaugeFloat64,
			},
			expected: metrictest.NewDescriptor(
				"foo",
				sdkapi.GaugeObserverInstrumentKind,
				number.Float64Kind,
				instrument.WithDescription("bar"),
				instrument.WithUnit(unit.Milliseconds),
			),
		},
		{
			desc: "cumulative int64 dimensionless",
			input: metricdata.Descriptor{
				Name:        "foo",
				Description: "bar",
				Unit:        metricdata.UnitDimensionless,
				Type:        metricdata.TypeCumulativeInt64,
			},
			expected: metrictest.NewDescriptor(
				"foo",
				sdkapi.CounterObserverInstrumentKind,
				number.Int64Kind,
				instrument.WithDescription("bar"),
				instrument.WithUnit(unit.Dimensionless),
			),
		},
		{
			desc: "cumulative float64 dimensionless",
			input: metricdata.Descriptor{
				Name:        "foo",
				Description: "bar",
				Unit:        metricdata.UnitDimensionless,
				Type:        metricdata.TypeCumulativeFloat64,
			},
			expected: metrictest.NewDescriptor(
				"foo",
				sdkapi.CounterObserverInstrumentKind,
				number.Float64Kind,
				instrument.WithDescription("bar"),
				instrument.WithUnit(unit.Dimensionless),
			),
		},
		{
			desc: "incompatible TypeCumulativeDistribution",
			input: metricdata.Descriptor{
				Name:        "foo",
				Description: "bar",
				Type:        metricdata.TypeCumulativeDistribution,
			},
			expectedErr: errConversion,
		},
	} {
		t.Run(tc.desc, func(t *testing.T) {
			output, err := convertDescriptor(tc.input)
			if !errors.Is(err, tc.expectedErr) {
				t.Errorf("convertDescriptor(%v) = err(%v), want err(%v)", tc.input, err, tc.expectedErr)
			}
			if output != tc.expected {
				t.Errorf("convertDescriptor(%v) = %+v, want %+v", tc.input, output, tc.expected)
			}
		})
	}
}
