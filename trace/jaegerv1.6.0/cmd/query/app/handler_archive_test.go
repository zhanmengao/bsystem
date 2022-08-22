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
	"errors"
	"fmt"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/mock"

	"github.com/jaegertracing/jaeger/model"
	"github.com/jaegertracing/jaeger/storage/spanstore"
	spanstoremocks "github.com/jaegertracing/jaeger/storage/spanstore/mocks"
)

func TestGetArchivedTrace_NotFound(t *testing.T) {
	mockReader := &spanstoremocks.Reader{}
	mockReader.On("GetTrace", mock.AnythingOfType("model.TraceID")).
		Return(nil, spanstore.ErrTraceNotFound).Once()
	for _, tc := range []spanstore.Reader{nil, mockReader} {
		archiveReader := tc // capture loop var
		t.Run(fmt.Sprint(archiveReader), func(t *testing.T) {
			withTestServer(t, func(ts *testServer) {
				ts.spanReader.On("GetTrace", mock.AnythingOfType("model.TraceID")).
					Return(nil, spanstore.ErrTraceNotFound).Once()
				var response structuredResponse
				err := getJSON(ts.server.URL+"/api/traces/"+mockTraceID.String(), &response)
				assert.EqualError(t, err,
					`404 error from server: {"data":null,"total":0,"limit":0,"offset":0,"errors":[{"code":404,"msg":"trace not found"}]}`+"\n",
				)
			}, HandlerOptions.ArchiveSpanReader(archiveReader)) // nil is ok
		})
	}
}

func TestGetArchivedTraceSuccess(t *testing.T) {
	traceID := model.NewTraceID(0, 123456)
	mockReader := &spanstoremocks.Reader{}
	mockReader.On("GetTrace", mock.AnythingOfType("model.TraceID")).
		Return(mockTrace, nil).Once()
	withTestServer(t, func(ts *testServer) {
		// maeke main reader return NotFound
		ts.spanReader.On("GetTrace", mock.AnythingOfType("model.TraceID")).
			Return(nil, spanstore.ErrTraceNotFound).Once()
		var response structuredTraceResponse
		err := getJSON(ts.server.URL+"/api/traces/"+mockTraceID.String(), &response)
		assert.NoError(t, err)
		assert.Len(t, response.Errors, 0)
		assert.Len(t, response.Traces, 1)
		assert.Equal(t, traceID.String(), string(response.Traces[0].TraceID))
	}, HandlerOptions.ArchiveSpanReader(mockReader))
}

func TestArchiveTrace_NoStorage(t *testing.T) {
	withTestServer(t, func(ts *testServer) {
		var response structuredResponse
		err := postJSON(ts.server.URL+"/api/archive/"+mockTraceID.String(), []string{}, &response)
		assert.EqualError(t, err, `500 error from server: {"data":null,"total":0,"limit":0,"offset":0,"errors":[{"code":500,"msg":"archive span storage was not configured"}]}`+"\n")
	})
}

func TestArchiveTrace_Success(t *testing.T) {
	mockWriter := &spanstoremocks.Writer{}
	mockWriter.On("WriteSpan", mock.AnythingOfType("*model.Span")).
		Return(nil).Times(2)
	withTestServer(t, func(ts *testServer) {
		ts.spanReader.On("GetTrace", mock.AnythingOfType("model.TraceID")).
			Return(mockTrace, nil).Once()
		var response structuredResponse
		err := postJSON(ts.server.URL+"/api/archive/"+mockTraceID.String(), []string{}, &response)
		assert.NoError(t, err)
	}, HandlerOptions.ArchiveSpanWriter(mockWriter))
}

func TestArchiveTrace_WriteErrors(t *testing.T) {
	mockWriter := &spanstoremocks.Writer{}
	mockWriter.On("WriteSpan", mock.AnythingOfType("*model.Span")).
		Return(errors.New("cannot save")).Times(2)
	withTestServer(t, func(ts *testServer) {
		ts.spanReader.On("GetTrace", mock.AnythingOfType("model.TraceID")).
			Return(mockTrace, nil).Once()
		var response structuredResponse
		err := postJSON(ts.server.URL+"/api/archive/"+mockTraceID.String(), []string{}, &response)
		assert.EqualError(t, err, `500 error from server: {"data":null,"total":0,"limit":0,"offset":0,"errors":[{"code":500,"msg":"[cannot save, cannot save]"}]}`+"\n")
	}, HandlerOptions.ArchiveSpanWriter(mockWriter))
}
