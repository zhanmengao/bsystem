// Copyright (c) 2021 The Jaeger Authors.
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

package init

import (
	"errors"
	"net/http"
	"strings"
	"testing"

	"github.com/crossdock/crossdock-go/assert"
	"github.com/stretchr/testify/mock"

	"github.com/jaegertracing/jaeger/cmd/es-rollover/app"
	"github.com/jaegertracing/jaeger/pkg/es/client"
	"github.com/jaegertracing/jaeger/pkg/es/client/mocks"
)

func TestIndexCreateIfNotExist(t *testing.T) {
	const esErrResponse = `{"error":{"root_cause":[{"type":"resource_already_exists_exception","reason":"]"}],"type":"resource_already_exists_exception","reason":"request [/jaeger-*] contains unrecognized parameter: [help]"},"status":400}`

	tests := []struct {
		name          string
		returnErr     error
		expectedErr   error
		containsError string
	}{
		{
			name: "success",
		},
		{
			name:        "generic error",
			returnErr:   errors.New("may be an http error?"),
			expectedErr: errors.New("may be an http error?"),
		},
		{
			name: "response error",
			returnErr: client.ResponseError{
				Err:        errors.New("x"),
				StatusCode: http.StatusForbidden,
			},
			expectedErr: errors.New("x"),
		},
		{
			name: "unmarshal error",
			returnErr: client.ResponseError{
				Err:        errors.New("x"),
				StatusCode: http.StatusBadRequest,
				Body:       []byte("blablabla"),
			},
			containsError: "invalid character",
		},
		{
			name: "existing error",
			returnErr: client.ResponseError{
				Err:        errors.New("x"),
				StatusCode: http.StatusBadRequest,
				Body:       []byte(esErrResponse),
			},
			expectedErr: nil,
		},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			indexClient := &mocks.MockIndexAPI{}
			indexClient.On("CreateIndex", "jaeger-span").Return(test.returnErr)
			err := createIndexIfNotExist(indexClient, "jaeger-span")
			if test.containsError != "" {
				assert.True(t, strings.Contains(err.Error(), test.containsError))
			} else {
				assert.Equal(t, test.expectedErr, err)
			}
		})
	}
}

func TestRolloverAction(t *testing.T) {
	tests := []struct {
		name                  string
		setupCallExpectations func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI)
		config                Config
		expectedErr           error
	}{
		{
			name: "Unsupported version",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(5), nil)
			},
			config: Config{
				Config: app.Config{
					Archive: true,
					UseILM:  true,
				},
			},
			expectedErr: errors.New("ILM is supported only for ES version 7+"),
		},
		{
			name: "error getting version",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(0), errors.New("version error"))
			},
			expectedErr: errors.New("version error"),
			config: Config{
				Config: app.Config{
					Archive: true,
					UseILM:  true,
				},
			},
		},
		{
			name: "ilm doesnt exist",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				ilmClient.On("Exists", "myilmpolicy").Return(false, nil)
			},
			expectedErr: errors.New("ILM policy myilmpolicy doesn't exist in Elasticsearch. Please create it and re-run init"),
			config: Config{
				Config: app.Config{
					Archive:       true,
					UseILM:        true,
					ILMPolicyName: "myilmpolicy",
				},
			},
		},
		{
			name: "fail get ilm policy",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				ilmClient.On("Exists", "myilmpolicy").Return(false, errors.New("error getting ilm policy"))
			},
			expectedErr: errors.New("error getting ilm policy"),
			config: Config{
				Config: app.Config{
					Archive:       true,
					UseILM:        true,
					ILMPolicyName: "myilmpolicy",
				},
			},
		},
		{
			name: "fail to create template",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				indexClient.On("CreateTemplate", mock.Anything, "jaeger-span").Return(errors.New("error creating template"))
			},
			expectedErr: errors.New("error creating template"),
			config: Config{
				Config: app.Config{
					Archive: true,
					UseILM:  false,
				},
			},
		},
		{
			name: "fail to get jaeger indices",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				indexClient.On("CreateTemplate", mock.Anything, "jaeger-span").Return(nil)
				indexClient.On("CreateIndex", "jaeger-span-archive-000001").Return(nil)
				indexClient.On("GetJaegerIndices", "").Return([]client.Index{}, errors.New("error getting jaeger indices"))
			},
			expectedErr: errors.New("error getting jaeger indices"),
			config: Config{
				Config: app.Config{
					Archive: true,
					UseILM:  false,
				},
			},
		},
		{
			name: "fail to create alias",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				indexClient.On("CreateTemplate", mock.Anything, "jaeger-span").Return(nil)
				indexClient.On("CreateIndex", "jaeger-span-archive-000001").Return(nil)
				indexClient.On("GetJaegerIndices", "").Return([]client.Index{}, nil)
				indexClient.On("CreateAlias", []client.Alias{
					{Index: "jaeger-span-archive-000001", Name: "jaeger-span-archive-read", IsWriteIndex: false},
					{Index: "jaeger-span-archive-000001", Name: "jaeger-span-archive-write", IsWriteIndex: false},
				}).Return(errors.New("error creating aliases"))
			},
			expectedErr: errors.New("error creating aliases"),
			config: Config{
				Config: app.Config{
					Archive: true,
					UseILM:  false,
				},
			},
		},
		{
			name: "create rollover index",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				indexClient.On("CreateTemplate", mock.Anything, "jaeger-span").Return(nil)
				indexClient.On("CreateIndex", "jaeger-span-archive-000001").Return(nil)
				indexClient.On("GetJaegerIndices", "").Return([]client.Index{}, nil)
				indexClient.On("CreateAlias", []client.Alias{
					{Index: "jaeger-span-archive-000001", Name: "jaeger-span-archive-read", IsWriteIndex: false},
					{Index: "jaeger-span-archive-000001", Name: "jaeger-span-archive-write", IsWriteIndex: false},
				}).Return(nil)
			},
			expectedErr: nil,
			config: Config{
				Config: app.Config{
					Archive: true,
					UseILM:  false,
				},
			},
		},
		{
			name: "create rollover index with ilm",
			setupCallExpectations: func(indexClient *mocks.MockIndexAPI, clusterClient *mocks.MockClusterAPI, ilmClient *mocks.MockILMAPI) {
				clusterClient.On("Version").Return(uint(7), nil)
				indexClient.On("CreateTemplate", mock.Anything, "jaeger-span").Return(nil)
				indexClient.On("CreateIndex", "jaeger-span-archive-000001").Return(nil)
				indexClient.On("GetJaegerIndices", "").Return([]client.Index{}, nil)
				ilmClient.On("Exists", "jaeger-ilm").Return(true, nil)
				indexClient.On("CreateAlias", []client.Alias{
					{Index: "jaeger-span-archive-000001", Name: "jaeger-span-archive-read", IsWriteIndex: false},
					{Index: "jaeger-span-archive-000001", Name: "jaeger-span-archive-write", IsWriteIndex: true},
				}).Return(nil)
			},
			expectedErr: nil,
			config: Config{
				Config: app.Config{
					Archive:       true,
					UseILM:        true,
					ILMPolicyName: "jaeger-ilm",
				},
			},
		},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			indexClient := &mocks.MockIndexAPI{}
			clusterClient := &mocks.MockClusterAPI{}
			ilmClient := &mocks.MockILMAPI{}
			initAction := Action{
				Config:        test.config,
				IndicesClient: indexClient,
				ClusterClient: clusterClient,
				ILMClient:     ilmClient,
			}

			test.setupCallExpectations(indexClient, clusterClient, ilmClient)

			err := initAction.Do()
			if test.expectedErr != nil {
				assert.Error(t, err)
				assert.Equal(t, test.expectedErr, err)
			}

			indexClient.AssertExpectations(t)
			clusterClient.AssertExpectations(t)
			ilmClient.AssertExpectations(t)
		})
	}
}
