// Copyright 2017 Google Inc.
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

#include "google/cloud/bigquery/internal/build_info.h"
#include "google/cloud/internal/port_platform.h"
#include <algorithm>
#include <cctype>
#include <iterator>

namespace google {
namespace cloud {
namespace bigquery {
inline namespace BIGQUERY_CLIENT_NS {
namespace internal {

std::string BuildFlags() {
  static auto const* const kFlags = [] {
    auto* flags = new std::string(R"""()""");
    if (!flags->empty()) {
      *flags += ' ';
    }
    *flags += R"""()""";
    return flags;
  }();
  return *kFlags;
}

std::string BuildMetadata() { return R"""(2ebb053)"""; }

}  // namespace internal
}  // namespace BIGQUERY_CLIENT_NS
}  // namespace bigquery
}  // namespace cloud
}  // namespace google
