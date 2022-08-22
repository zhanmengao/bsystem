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

package filter

import (
	"time"

	"github.com/jaegertracing/jaeger/pkg/es/client"
)

// ByDate filter indices by creationTime, return indices that were created before certain date.
func ByDate(indices []client.Index, beforeThisDate time.Time) []client.Index {
	var filtered []client.Index
	for _, in := range indices {
		if in.CreationTime.Before(beforeThisDate) {
			filtered = append(filtered, in)
		}
	}
	return filtered
}
