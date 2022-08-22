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

package spanstore

import (
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"
	"github.com/stretchr/testify/require"
	"gopkg.in/olivere/elastic.v5"

	jModel "github.com/jaegertracing/jaeger/model/json"
	"github.com/jaegertracing/jaeger/pkg/es/mocks"
)

func TestWriteService(t *testing.T) {
	withSpanWriter(func(w *spanWriterTest) {
		indexService := &mocks.IndexService{}

		indexName := "jaeger-1995-04-21"
		serviceHash := "de3b5a8f1a79989d"

		indexService.On("Index", stringMatcher(indexName)).Return(indexService)
		indexService.On("Type", stringMatcher(serviceType)).Return(indexService)
		indexService.On("Id", stringMatcher(serviceHash)).Return(indexService)
		indexService.On("BodyJson", mock.AnythingOfType("spanstore.Service")).Return(indexService)
		indexService.On("Add")

		w.client.On("Index").Return(indexService)

		jsonSpan := &jModel.Span{
			TraceID:       jModel.TraceID("1"),
			SpanID:        jModel.SpanID("0"),
			OperationName: "operation",
			Process: &jModel.Process{
				ServiceName: "service",
			},
		}

		w.writer.writeService(indexName, jsonSpan)

		indexService.AssertNumberOfCalls(t, "Add", 1)
		assert.Equal(t, "", w.logBuffer.String())

		// test that cache works, will call the index service only once.
		w.writer.writeService(indexName, jsonSpan)
		indexService.AssertNumberOfCalls(t, "Add", 1)
	})
}

func TestWriteServiceError(t *testing.T) {
	withSpanWriter(func(w *spanWriterTest) {
		indexService := &mocks.IndexService{}

		indexName := "jaeger-1995-04-21"
		serviceHash := "de3b5a8f1a79989d"

		indexService.On("Index", stringMatcher(indexName)).Return(indexService)
		indexService.On("Type", stringMatcher(serviceType)).Return(indexService)
		indexService.On("Id", stringMatcher(serviceHash)).Return(indexService)
		indexService.On("BodyJson", mock.AnythingOfType("spanstore.Service")).Return(indexService)
		indexService.On("Add")

		w.client.On("Index").Return(indexService)

		jsonSpan := &jModel.Span{
			TraceID:       jModel.TraceID("1"),
			SpanID:        jModel.SpanID("0"),
			OperationName: "operation",
			Process: &jModel.Process{
				ServiceName: "service",
			},
		}

		w.writer.writeService(indexName, jsonSpan)
	})
}

func TestSpanReader_GetServices(t *testing.T) {
	testGet(servicesAggregation, t)
}

func TestSpanReader_GetOperations(t *testing.T) {
	testGet(operationsAggregation, t)
}

func TestSpanReader_GetServicesEmptyIndex(t *testing.T) {
	withSpanReader(func(r *spanReaderTest) {
		mockSearchService(r).
			Return(&elastic.SearchResult{}, nil)
		mockMultiSearchService(r).
			Return(&elastic.MultiSearchResult{
				Responses: []*elastic.SearchResult{},
			}, nil)
		services, err := r.reader.GetServices()
		require.NoError(t, err)
		assert.Empty(t, services)
	})
}

func TestSpanReader_GetOperationsEmptyIndex(t *testing.T) {
	withSpanReader(func(r *spanReaderTest) {
		mockSearchService(r).
			Return(&elastic.SearchResult{}, nil)
		mockMultiSearchService(r).
			Return(&elastic.MultiSearchResult{
				Responses: []*elastic.SearchResult{},
			}, nil)
		services, err := r.reader.GetOperations("foo")
		require.NoError(t, err)
		assert.Empty(t, services)
	})
}
