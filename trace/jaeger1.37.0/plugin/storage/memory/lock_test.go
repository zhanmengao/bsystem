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

package memory

import (
	"testing"
	"time"

	"github.com/crossdock/crossdock-go/assert"
)

func TestAcquire(t *testing.T) {
	l := &lock{}
	ok, err := l.Acquire("resource", time.Duration(1))
	assert.True(t, ok)
	assert.NoError(t, err)
}

func TestForfeit(t *testing.T) {
	l := &lock{}
	ok, err := l.Forfeit("resource")
	assert.True(t, ok)
	assert.NoError(t, err)
}
