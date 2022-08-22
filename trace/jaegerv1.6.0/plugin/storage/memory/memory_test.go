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

package memory

import (
	"testing"
	"time"

	"github.com/stretchr/testify/assert"

	"github.com/jaegertracing/jaeger/model"
	"github.com/jaegertracing/jaeger/pkg/memory/config"
	"github.com/jaegertracing/jaeger/storage/spanstore"
)

var traceID = model.NewTraceID(1, 2)

var testingSpan = &model.Span{
	TraceID: traceID,
	SpanID:  model.NewSpanID(1),
	Process: &model.Process{
		ServiceName: "serviceName",
		Tags:        model.KeyValues{},
	},
	OperationName: "operationName",
	Tags: model.KeyValues{
		model.String("tagKey", "tagValue"),
	},
	Logs: []model.Log{
		{
			Timestamp: time.Now(),
			Fields: []model.KeyValue{
				model.String("logKey", "logValue"),
			},
		},
	},
	Duration:  time.Second * 5,
	StartTime: time.Unix(300, 0),
}

var childSpan1 = &model.Span{
	TraceID:    traceID,
	SpanID:     model.NewSpanID(2),
	References: []model.SpanRef{model.NewChildOfRef(traceID, model.NewSpanID(1))},
	Process: &model.Process{
		ServiceName: "childService",
		Tags:        model.KeyValues{},
	},
	OperationName: "childOperationName",
	Tags: model.KeyValues{
		model.String("tagKey", "tagValue"),
	},
	Logs: []model.Log{
		{
			Timestamp: time.Now(),
			Fields: []model.KeyValue{
				model.String("logKey", "logValue"),
			},
		},
	},
	Duration:  time.Second * 5,
	StartTime: time.Unix(300, 0),
}

var childSpan2 = &model.Span{
	TraceID:    traceID,
	SpanID:     model.NewSpanID(3),
	References: []model.SpanRef{model.NewChildOfRef(traceID, model.NewSpanID(1))},
	Process: &model.Process{
		ServiceName: "childService",
		Tags:        model.KeyValues{},
	},
	OperationName: "childOperationName",
	Tags: model.KeyValues{
		model.String("tagKey", "tagValue"),
	},
	Logs: []model.Log{
		{
			Timestamp: time.Now(),
			Fields: []model.KeyValue{
				model.String("logKey", "logValue"),
			},
		},
	},
	Duration:  time.Second * 5,
	StartTime: time.Unix(300, 0),
}

var childSpan2_1 = &model.Span{
	TraceID: traceID,
	SpanID:  model.NewSpanID(4),
	// child of childSpan2, but with the same service name
	References: []model.SpanRef{model.NewChildOfRef(traceID, model.NewSpanID(3))},
	Process: &model.Process{
		ServiceName: "childService",
		Tags:        model.KeyValues{},
	},
	OperationName: "childOperationName",
	Tags: model.KeyValues{
		model.String("tagKey", "tagValue"),
	},
	Logs: []model.Log{
		{
			Timestamp: time.Now(),
			Fields: []model.KeyValue{
				model.String("logKey", "logValue"),
			},
		},
	},
	Duration:  time.Second * 5,
	StartTime: time.Unix(300, 0),
}

func withPopulatedMemoryStore(f func(store *Store)) {
	memStore := NewStore()
	memStore.WriteSpan(testingSpan)
	f(memStore)
}
func withMemoryStore(f func(store *Store)) {
	f(NewStore())
}

func TestStoreGetEmptyDependencies(t *testing.T) {
	withMemoryStore(func(store *Store) {
		links, err := store.GetDependencies(time.Now(), time.Hour)
		assert.NoError(t, err)
		assert.Empty(t, links)
	})
}

func TestStoreGetDependencies(t *testing.T) {
	withMemoryStore(func(store *Store) {
		assert.NoError(t, store.WriteSpan(testingSpan))
		assert.NoError(t, store.WriteSpan(childSpan1))
		assert.NoError(t, store.WriteSpan(childSpan2))
		assert.NoError(t, store.WriteSpan(childSpan2_1))
		links, err := store.GetDependencies(time.Now(), time.Hour)
		assert.NoError(t, err)
		assert.Empty(t, links)

		links, err = store.GetDependencies(time.Unix(0, 0).Add(time.Hour), time.Hour)
		assert.NoError(t, err)
		assert.Equal(t, []model.DependencyLink{{
			Parent:    "serviceName",
			Child:     "childService",
			CallCount: 2,
		}}, links)
	})
}

func TestStoreWriteSpan(t *testing.T) {
	withMemoryStore(func(store *Store) {
		err := store.WriteSpan(testingSpan)
		assert.NoError(t, err)
	})
}

func TestStoreWithLimit(t *testing.T) {
	maxTraces := 100
	store := WithConfiguration(config.Configuration{MaxTraces: maxTraces})

	for i := 0; i < maxTraces*2; i++ {
		id := model.NewTraceID(1, uint64(i))
		err := store.WriteSpan(&model.Span{
			TraceID: id,
			Process: &model.Process{
				ServiceName: "TestStoreWithLimit",
			},
		})
		assert.NoError(t, err)

		err = store.WriteSpan(&model.Span{
			TraceID: id,
			SpanID:  model.NewSpanID(uint64(i)),
			Process: &model.Process{
				ServiceName: "TestStoreWithLimit",
			},
			OperationName: "childOperationName",
		})
		assert.NoError(t, err)
	}

	assert.Equal(t, maxTraces, len(store.traces))
	assert.Equal(t, maxTraces, len(store.ids))
}

func TestStoreGetTraceSuccess(t *testing.T) {
	withPopulatedMemoryStore(func(store *Store) {
		trace, err := store.GetTrace(testingSpan.TraceID)
		assert.NoError(t, err)
		assert.Len(t, trace.Spans, 1)
		assert.Equal(t, testingSpan, trace.Spans[0])
	})
}

func TestStoreGetTraceFailure(t *testing.T) {
	withPopulatedMemoryStore(func(store *Store) {
		trace, err := store.GetTrace(model.TraceID{})
		assert.EqualError(t, err, errTraceNotFound.Error())
		assert.Nil(t, trace)
	})
}

func TestStoreGetServices(t *testing.T) {
	withPopulatedMemoryStore(func(store *Store) {
		serviceNames, err := store.GetServices()
		assert.NoError(t, err)
		assert.Len(t, serviceNames, 1)
		assert.EqualValues(t, testingSpan.Process.ServiceName, serviceNames[0])
	})
}

func TestStoreGetOperationsFound(t *testing.T) {
	withPopulatedMemoryStore(func(store *Store) {
		operations, err := store.GetOperations(testingSpan.Process.ServiceName)
		assert.NoError(t, err)
		assert.Len(t, operations, 1)
		assert.EqualValues(t, testingSpan.OperationName, operations[0])
	})
}

func TestStoreGetOperationsNotFound(t *testing.T) {
	withPopulatedMemoryStore(func(store *Store) {
		operations, err := store.GetOperations("notAService")
		assert.NoError(t, err)
		assert.Len(t, operations, 0)
	})
}

func TestStoreGetEmptyTraceSet(t *testing.T) {
	withPopulatedMemoryStore(func(store *Store) {
		traces, err := store.FindTraces(&spanstore.TraceQueryParameters{})
		assert.NoError(t, err)
		assert.Len(t, traces, 0)
	})
}

func TestStoreGetTrace(t *testing.T) {
	testStruct := []struct {
		query      *spanstore.TraceQueryParameters
		traceFound bool
	}{
		{
			&spanstore.TraceQueryParameters{
				ServiceName: testingSpan.Process.ServiceName,
			}, true,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName: "wrongServiceName",
			}, false,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName:   testingSpan.Process.ServiceName,
				OperationName: "wrongOperationName",
			}, false,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName: testingSpan.Process.ServiceName,
				DurationMin: time.Second * 10,
			}, false,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName: testingSpan.Process.ServiceName,
				DurationMax: time.Second * 2,
			}, false,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName:  testingSpan.Process.ServiceName,
				StartTimeMin: time.Unix(500, 0),
			}, false,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName:  testingSpan.Process.ServiceName,
				StartTimeMax: time.Unix(100, 0),
			}, false,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName: testingSpan.Process.ServiceName,
				Tags: map[string]string{
					testingSpan.Tags[0].Key:           testingSpan.Tags[0].VStr,
					testingSpan.Logs[0].Fields[0].Key: testingSpan.Logs[0].Fields[0].VStr,
				},
			}, true,
		},
		{
			&spanstore.TraceQueryParameters{
				ServiceName: testingSpan.Process.ServiceName,
				Tags: map[string]string{
					testingSpan.Tags[0].Key: testingSpan.Logs[0].Fields[0].VStr,
				},
			}, false,
		},
	}
	for _, testS := range testStruct {
		withPopulatedMemoryStore(func(store *Store) {
			testS.query.NumTraces = 10
			traces, err := store.FindTraces(testS.query)
			assert.NoError(t, err)
			if testS.traceFound {
				assert.Len(t, traces, 1)
				assert.Len(t, traces[0].Spans, 1)
				assert.Equal(t, testingSpan, traces[0].Spans[0])
			} else {
				assert.Empty(t, traces)
			}
		})
	}
}
