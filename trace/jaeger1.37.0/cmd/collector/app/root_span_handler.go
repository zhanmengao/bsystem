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

package app

import (
	"go.uber.org/zap"

	"github.com/jaegertracing/jaeger/cmd/collector/app/sampling/strategystore"
	"github.com/jaegertracing/jaeger/model"
)

// handleRootSpan returns a function that records throughput for root spans
func handleRootSpan(aggregator strategystore.Aggregator, logger *zap.Logger) ProcessSpan {
	return func(span *model.Span, tenant string) {
		// TODO simply checking parentId to determine if a span is a root span is not sufficient. However,
		// we can be sure that only a root span will have sampler tags.
		if span.ParentSpanID() != model.NewSpanID(0) {
			return
		}
		service := span.Process.ServiceName
		if service == "" || span.OperationName == "" {
			return
		}
		samplerType, samplerParam := span.GetSamplerParams(logger)
		if samplerType == "" {
			return
		}
		aggregator.RecordThroughput(service, span.OperationName, samplerType, samplerParam)
	}
}
