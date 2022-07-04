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

package otlpmetrichttp // import "go.opentelemetry.io/otel/exporters/otlp/otlpmetric/otlpmetrichttp"

import (
	"crypto/tls"
	"time"

	"go.opentelemetry.io/otel/exporters/otlp/internal/retry"
	"go.opentelemetry.io/otel/exporters/otlp/otlpmetric/internal/otlpconfig"
)

// Compression describes the compression used for payloads sent to the
// collector.
type Compression otlpconfig.Compression

const (
	// NoCompression tells the driver to send payloads without
	// compression.
	NoCompression = Compression(otlpconfig.NoCompression)
	// GzipCompression tells the driver to send payloads after
	// compressing them with gzip.
	GzipCompression = Compression(otlpconfig.GzipCompression)
)

// Option applies an option to the HTTP client.
type Option interface {
	applyHTTPOption(otlpconfig.Config) otlpconfig.Config
}

func asHTTPOptions(opts []Option) []otlpconfig.HTTPOption {
	converted := make([]otlpconfig.HTTPOption, len(opts))
	for i, o := range opts {
		converted[i] = otlpconfig.NewHTTPOption(o.applyHTTPOption)
	}
	return converted
}

// RetryConfig defines configuration for retrying batches in case of export
// failure using an exponential backoff.
type RetryConfig retry.Config

type wrappedOption struct {
	otlpconfig.HTTPOption
}

func (w wrappedOption) applyHTTPOption(cfg otlpconfig.Config) otlpconfig.Config {
	return w.ApplyHTTPOption(cfg)
}

// WithEndpoint allows one to set the address of the collector endpoint that
// the driver will use to send metrics. If unset, it will instead try to use
// the default endpoint (localhost:4318). Note that the endpoint must not
// contain any URL path.
func WithEndpoint(endpoint string) Option {
	return wrappedOption{otlpconfig.WithEndpoint(endpoint)}
}

// WithCompression tells the driver to compress the sent data.
func WithCompression(compression Compression) Option {
	return wrappedOption{otlpconfig.WithCompression(otlpconfig.Compression(compression))}
}

// WithURLPath allows one to override the default URL path used
// for sending metrics. If unset, default ("/v1/metrics") will be used.
func WithURLPath(urlPath string) Option {
	return wrappedOption{otlpconfig.WithURLPath(urlPath)}
}

// WithMaxAttempts allows one to override how many times the driver
// will try to send the payload in case of retryable errors.
// The max attempts is limited to at most 5 retries. If unset,
// default (5) will be used.
//
// Deprecated: Use WithRetry instead.
func WithMaxAttempts(maxAttempts int) Option {
	if maxAttempts > 5 || maxAttempts < 0 {
		maxAttempts = 5
	}
	return wrappedOption{
		otlpconfig.NewHTTPOption(func(cfg otlpconfig.Config) otlpconfig.Config {
			cfg.RetryConfig.Enabled = true

			var (
				init = cfg.RetryConfig.InitialInterval
				maxI = cfg.RetryConfig.MaxInterval
				maxE = cfg.RetryConfig.MaxElapsedTime
			)

			if init == 0 {
				init = retry.DefaultConfig.InitialInterval
			}
			if maxI == 0 {
				maxI = retry.DefaultConfig.MaxInterval
			}
			if maxE == 0 {
				maxE = retry.DefaultConfig.MaxElapsedTime
			}
			attempts := int64(maxE+init) / int64(maxI)

			if int64(maxAttempts) == attempts {
				return cfg
			}

			maxE = time.Duration(int64(maxAttempts)*int64(maxI)) - init

			cfg.RetryConfig.InitialInterval = init
			cfg.RetryConfig.MaxInterval = maxI
			cfg.RetryConfig.MaxElapsedTime = maxE

			return cfg
		}),
	}
}

// WithBackoff tells the driver to use the duration as a base of the
// exponential backoff strategy. If unset, default (300ms) will be
// used.
//
// Deprecated: Use WithRetry instead.
func WithBackoff(duration time.Duration) Option {
	if duration < 0 {
		duration = 300 * time.Millisecond
	}
	return wrappedOption{
		otlpconfig.NewHTTPOption(func(cfg otlpconfig.Config) otlpconfig.Config {
			cfg.RetryConfig.Enabled = true
			cfg.RetryConfig.MaxInterval = duration
			if cfg.RetryConfig.InitialInterval == 0 {
				cfg.RetryConfig.InitialInterval = retry.DefaultConfig.InitialInterval
			}
			if cfg.RetryConfig.MaxElapsedTime == 0 {
				cfg.RetryConfig.MaxElapsedTime = retry.DefaultConfig.MaxElapsedTime
			}
			return cfg
		}),
	}
}

// WithTLSClientConfig can be used to set up a custom TLS
// configuration for the client used to send payloads to the
// collector. Use it if you want to use a custom certificate.
func WithTLSClientConfig(tlsCfg *tls.Config) Option {
	return wrappedOption{otlpconfig.WithTLSClientConfig(tlsCfg)}
}

// WithInsecure tells the driver to connect to the collector using the
// HTTP scheme, instead of HTTPS.
func WithInsecure() Option {
	return wrappedOption{otlpconfig.WithInsecure()}
}

// WithHeaders allows one to tell the driver to send additional HTTP
// headers with the payloads. Specifying headers like Content-Length,
// Content-Encoding and Content-Type may result in a broken driver.
func WithHeaders(headers map[string]string) Option {
	return wrappedOption{otlpconfig.WithHeaders(headers)}
}

// WithTimeout tells the driver the max waiting time for the backend to process
// each metrics batch.  If unset, the default will be 10 seconds.
func WithTimeout(duration time.Duration) Option {
	return wrappedOption{otlpconfig.WithTimeout(duration)}
}

// WithRetry configures the retry policy for transient errors that may occurs
// when exporting traces. An exponential back-off algorithm is used to ensure
// endpoints are not overwhelmed with retries. If unset, the default retry
// policy will retry after 5 seconds and increase exponentially after each
// error for a total of 1 minute.
func WithRetry(rc RetryConfig) Option {
	return wrappedOption{otlpconfig.WithRetry(retry.Config(rc))}
}
