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
)

// Store results in a file scope var to ensure compiler does not optimize the
// test away.
var (
	outV  Value
	outKV KeyValue

	outBool         bool
	outBoolSlice    []bool
	outInt64        int64
	outInt64Slice   []int64
	outFloat64      float64
	outFloat64Slice []float64
	outStr          string
	outStrSlice     []string
)

func benchmarkEmit(kv KeyValue) func(*testing.B) {
	return func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outStr = kv.Value.Emit()
		}
	}
}

func BenchmarkBool(b *testing.B) {
	k, v := "bool", true
	kv := Bool(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = BoolValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = Bool(k, v)
		}
	})
	b.Run("AsBool", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outBool = kv.Value.AsBool()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkBoolSlice(b *testing.B) {
	k, v := "bool slice", []bool{true, false, true}
	kv := BoolSlice(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = BoolSliceValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = BoolSlice(k, v)
		}
	})
	b.Run("AsBoolSlice", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outBoolSlice = kv.Value.AsBoolSlice()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkInt(b *testing.B) {
	k, v := "int", int(42)
	kv := Int(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = IntValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = Int(k, v)
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkIntSlice(b *testing.B) {
	k, v := "int slice", []int{42, -3, 12}
	kv := IntSlice(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = IntSliceValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = IntSlice(k, v)
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkInt64(b *testing.B) {
	k, v := "int64", int64(42)
	kv := Int64(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = Int64Value(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = Int64(k, v)
		}
	})
	b.Run("AsInt64", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outInt64 = kv.Value.AsInt64()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkInt64Slice(b *testing.B) {
	k, v := "int64 slice", []int64{42, -3, 12}
	kv := Int64Slice(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = Int64SliceValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = Int64Slice(k, v)
		}
	})
	b.Run("AsInt64Slice", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outInt64Slice = kv.Value.AsInt64Slice()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkFloat64(b *testing.B) {
	k, v := "float64", float64(42)
	kv := Float64(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = Float64Value(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = Float64(k, v)
		}
	})
	b.Run("AsFloat64", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outFloat64 = kv.Value.AsFloat64()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkFloat64Slice(b *testing.B) {
	k, v := "float64 slice", []float64{42, -3, 12}
	kv := Float64Slice(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = Float64SliceValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = Float64Slice(k, v)
		}
	})
	b.Run("AsFloat64Slice", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outFloat64Slice = kv.Value.AsFloat64Slice()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkString(b *testing.B) {
	k, v := "string", "42"
	kv := String(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = StringValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = String(k, v)
		}
	})
	b.Run("AsString", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outStr = kv.Value.AsString()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}

func BenchmarkStringSlice(b *testing.B) {
	k, v := "float64 slice", []string{"forty-two", "negative three", "twelve"}
	kv := StringSlice(k, v)

	b.Run("Value", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outV = StringSliceValue(v)
		}
	})
	b.Run("KeyValue", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outKV = StringSlice(k, v)
		}
	})
	b.Run("AsStringSlice", func(b *testing.B) {
		b.ReportAllocs()
		for i := 0; i < b.N; i++ {
			outStrSlice = kv.Value.AsStringSlice()
		}
	})
	b.Run("Emit", benchmarkEmit(kv))
}
