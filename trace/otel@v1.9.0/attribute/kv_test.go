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

package attribute_test

import (
	"testing"

	"github.com/google/go-cmp/cmp"
)

func TestKeyValueConstructors(t *testing.T) {
	tt := []struct {
		name     string
		actual   KeyValue
		expected KeyValue
	}{
		{
			name:   "Bool",
			actual: Bool("k1", true),
			expected: KeyValue{
				Key:   "k1",
				Value: BoolValue(true),
			},
		},
		{
			name:   "Int64",
			actual: Int64("k1", 123),
			expected: KeyValue{
				Key:   "k1",
				Value: Int64Value(123),
			},
		},
		{
			name:   "Float64",
			actual: Float64("k1", 123.5),
			expected: KeyValue{
				Key:   "k1",
				Value: Float64Value(123.5),
			},
		},
		{
			name:   "String",
			actual: String("k1", "123.5"),
			expected: KeyValue{
				Key:   "k1",
				Value: StringValue("123.5"),
			},
		},
		{
			name:   "Int",
			actual: Int("k1", 123),
			expected: KeyValue{
				Key:   "k1",
				Value: IntValue(123),
			},
		},
	}

	for _, test := range tt {
		t.Run(test.name, func(t *testing.T) {
			if diff := cmp.Diff(test.actual, test.expected, cmp.AllowUnexported(Value{})); diff != "" {
				t.Fatal(diff)
			}
		})
	}
}

func TestKeyValueValid(t *testing.T) {
	tests := []struct {
		desc  string
		valid bool
		kv    KeyValue
	}{
		{
			desc:  "uninitialized KeyValue should be invalid",
			valid: false,
			kv:    KeyValue{},
		},
		{
			desc:  "empty key value should be invalid",
			valid: false,
			kv:    Key("").Bool(true),
		},
		{
			desc:  "INVALID value type should be invalid",
			valid: false,
			kv: KeyValue{
				Key: Key("valid key"),
				// Default type is INVALID.
				Value: Value{},
			},
		},
		{
			desc:  "non-empty key with BOOL type Value should be valid",
			valid: true,
			kv:    Bool("bool", true),
		},
		{
			desc:  "non-empty key with INT64 type Value should be valid",
			valid: true,
			kv:    Int64("int64", 0),
		},
		{
			desc:  "non-empty key with FLOAT64 type Value should be valid",
			valid: true,
			kv:    Float64("float64", 0),
		},
		{
			desc:  "non-empty key with STRING type Value should be valid",
			valid: true,
			kv:    String("string", ""),
		},
	}

	for _, test := range tests {
		if got, want := test.kv.Valid(), test.valid; got != want {
			t.Error(test.desc)
		}
	}
}
