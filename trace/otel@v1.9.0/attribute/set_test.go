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
	"regexp"
	"testing"

	"github.com/stretchr/testify/require"
)

type testCase struct {
	kvs []KeyValue

	keyRe *regexp.Regexp

	encoding string
	fullEnc  string
}

func expect(enc string, kvs ...KeyValue) testCase {
	return testCase{
		kvs:      kvs,
		encoding: enc,
	}
}

func expectFiltered(enc, filter, fullEnc string, kvs ...KeyValue) testCase {
	return testCase{
		kvs:      kvs,
		keyRe:    regexp.MustCompile(filter),
		encoding: enc,
		fullEnc:  fullEnc,
	}
}

func TestSetDedup(t *testing.T) {
	cases := []testCase{
		expect("A=B", String("A", "2"), String("A", "B")),
		expect("A=B", String("A", "2"), Int("A", 1), String("A", "B")),
		expect("A=B", String("A", "B"), String("A", "C"), String("A", "D"), String("A", "B")),

		expect("A=B,C=D", String("A", "1"), String("C", "D"), String("A", "B")),
		expect("A=B,C=D", String("A", "2"), String("A", "B"), String("C", "D")),
		expect("A=B,C=D", Float64("C", 1.2), String("A", "2"), String("A", "B"), String("C", "D")),
		expect("A=B,C=D", String("C", "D"), String("A", "B"), String("A", "C"), String("A", "D"), String("A", "B")),
		expect("A=B,C=D", String("A", "B"), String("C", "D"), String("A", "C"), String("A", "D"), String("A", "B")),
		expect("A=B,C=D", String("A", "B"), String("A", "C"), String("A", "D"), String("A", "B"), String("C", "D")),
	}
	enc := DefaultEncoder()

	s2d := map[string][]Distinct{}
	d2s := map[Distinct][]string{}

	for _, tc := range cases {
		cpy := make([]KeyValue, len(tc.kvs))
		copy(cpy, tc.kvs)
		sl := NewSet(cpy...)

		// Ensure that the input was reordered but no elements went missing.
		require.ElementsMatch(t, tc.kvs, cpy)

		str := sl.Encoded(enc)
		equ := sl.Equivalent()

		s2d[str] = append(s2d[str], equ)
		d2s[equ] = append(d2s[equ], str)

		require.Equal(t, tc.encoding, str)
	}

	for s, d := range s2d {
		// No other Distinct values are equal to this.
		for s2, d2 := range s2d {
			if s2 == s {
				continue
			}
			for _, elt := range d {
				for _, otherDistinct := range d2 {
					require.NotEqual(t, otherDistinct, elt)
				}
			}
		}
		for _, strings := range d2s {
			if strings[0] == s {
				continue
			}
			for _, otherString := range strings {
				require.NotEqual(t, otherString, s)
			}
		}
	}

	for d, s := range d2s {
		// No other Distinct values are equal to this.
		for d2, s2 := range d2s {
			if d2 == d {
				continue
			}
			for _, elt := range s {
				for _, otherDistinct := range s2 {
					require.NotEqual(t, otherDistinct, elt)
				}
			}
		}
		for _, distincts := range s2d {
			if distincts[0] == d {
				continue
			}
			for _, otherDistinct := range distincts {
				require.NotEqual(t, otherDistinct, d)
			}
		}
	}
}

func TestUniqueness(t *testing.T) {
	short := []KeyValue{
		String("A", "0"),
		String("B", "2"),
		String("A", "1"),
	}
	long := []KeyValue{
		String("B", "2"),
		String("C", "5"),
		String("B", "2"),
		String("C", "1"),
		String("A", "4"),
		String("C", "3"),
		String("A", "1"),
	}
	cases := []testCase{
		expectFiltered("A=1", "^A$", "B=2", short...),
		expectFiltered("B=2", "^B$", "A=1", short...),
		expectFiltered("A=1,B=2", "^A|B$", "", short...),
		expectFiltered("", "^C", "A=1,B=2", short...),

		expectFiltered("A=1,C=3", "A|C", "B=2", long...),
		expectFiltered("B=2,C=3", "C|B", "A=1", long...),
		expectFiltered("C=3", "C", "A=1,B=2", long...),
		expectFiltered("", "D", "A=1,B=2,C=3", long...),
	}
	enc := DefaultEncoder()

	for _, tc := range cases {
		cpy := make([]KeyValue, len(tc.kvs))
		copy(cpy, tc.kvs)
		distinct, uniq := NewSetWithFiltered(cpy, func(attr KeyValue) bool {
			return tc.keyRe.MatchString(string(attr.Key))
		})

		full := NewSet(uniq...)

		require.Equal(t, tc.encoding, distinct.Encoded(enc))
		require.Equal(t, tc.fullEnc, full.Encoded(enc))
	}
}

func TestLookup(t *testing.T) {
	set := NewSet(Int("C", 3), Int("A", 1), Int("B", 2))

	value, has := set.Value("C")
	require.True(t, has)
	require.Equal(t, int64(3), value.AsInt64())

	value, has = set.Value("B")
	require.True(t, has)
	require.Equal(t, int64(2), value.AsInt64())

	value, has = set.Value("A")
	require.True(t, has)
	require.Equal(t, int64(1), value.AsInt64())

	value, has = set.Value("D")
	require.False(t, has)
}
