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

package trace

import (
	"fmt"
	"testing"

	"go.opentelemetry.io/otel/attribute"
)

const testKeyFmt = "test-key-%d"

func TestAttributesMap(t *testing.T) {
	wantCapacity := 128
	attrMap := newAttributesMap(wantCapacity)

	for i := 0; i < 256; i++ {
		attrMap.add(attribute.Int(fmt.Sprintf(testKeyFmt, i), i))
	}
	if attrMap.capacity != wantCapacity {
		t.Errorf("attrMap.capacity: got '%d'; want '%d'", attrMap.capacity, wantCapacity)
	}

	if attrMap.droppedCount != wantCapacity {
		t.Errorf("attrMap.droppedCount: got '%d'; want '%d'", attrMap.droppedCount, wantCapacity)
	}

	for i := 0; i < wantCapacity; i++ {
		key := attribute.Key(fmt.Sprintf(testKeyFmt, i))
		_, ok := attrMap.attributes[key]
		if ok {
			t.Errorf("key %q should be dropped", testKeyFmt)
		}
	}
	for i := wantCapacity; i < 256; i++ {
		key := attribute.Key(fmt.Sprintf(testKeyFmt, i))
		_, ok := attrMap.attributes[key]
		if !ok {
			t.Errorf("key %q should not be dropped", key)
		}
	}
}

func TestAttributesMapGetOldestRemoveOldest(t *testing.T) {
	attrMap := newAttributesMap(128)

	for i := 0; i < 128; i++ {
		attrMap.add(attribute.Int(fmt.Sprintf(testKeyFmt, i), i))
	}

	attrMap.removeOldest()
	attrMap.removeOldest()
	attrMap.removeOldest()

	for i := 0; i < 3; i++ {
		key := attribute.Key(fmt.Sprintf(testKeyFmt, i))
		_, ok := attrMap.attributes[key]
		if ok {
			t.Errorf("key %q should be removed", key)
		}
	}
}

func TestAttributesMapToKeyValue(t *testing.T) {
	attrMap := newAttributesMap(128)

	for i := 0; i < 128; i++ {
		attrMap.add(attribute.Int(fmt.Sprintf(testKeyFmt, i), i))
	}

	kv := attrMap.toKeyValue()

	gotAttrLen := len(kv)
	wantAttrLen := 128
	if gotAttrLen != wantAttrLen {
		t.Errorf("len(attrMap.attributes): got '%d'; want '%d'", gotAttrLen, wantAttrLen)
	}
}

func BenchmarkAttributesMapToKeyValue(b *testing.B) {
	attrMap := newAttributesMap(128)

	for i := 0; i < 128; i++ {
		attrMap.add(attribute.Int(fmt.Sprintf(testKeyFmt, i), i))
	}

	for n := 0; n < b.N; n++ {
		attrMap.toKeyValue()
	}
}
