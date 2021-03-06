// Copyright 2019 Google LLC
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

#include "google/cloud/storage/internal/policy_document_request.h"
#include "google/cloud/storage/internal/nljson.h"
#include "google/cloud/internal/parse_rfc3339.h"
#include <gmock/gmock.h>

namespace google {
namespace cloud {
namespace storage {
inline namespace STORAGE_CLIENT_NS {
namespace internal {
namespace {

TEST(PolicyDocumentRequest, SigningAccount) {
  PolicyDocumentRequest request;
  EXPECT_FALSE(request.signing_account().has_value());
  EXPECT_FALSE(request.signing_account_delegates().has_value());

  request.set_multiple_options(
      SigningAccount("another-account@example.com"),
      SigningAccountDelegates({"test-delegate1", "test-delegate2"}));
  ASSERT_TRUE(request.signing_account().has_value());
  ASSERT_TRUE(request.signing_account_delegates().has_value());
  EXPECT_EQ("another-account@example.com", request.signing_account().value());
  EXPECT_THAT(request.signing_account_delegates().value(),
              ::testing::ElementsAre("test-delegate1", "test-delegate2"));
}

TEST(PolicyDocumentV4Request, SigningAccount) {
  PolicyDocumentV4Request request;
  EXPECT_FALSE(request.signing_account().has_value());
  EXPECT_FALSE(request.signing_account_delegates().has_value());

  request.set_multiple_options(
      SigningAccount("another-account@example.com"),
      SigningAccountDelegates({"test-delegate1", "test-delegate2"}));
  ASSERT_TRUE(request.signing_account().has_value());
  ASSERT_TRUE(request.signing_account_delegates().has_value());
  EXPECT_EQ("another-account@example.com", request.signing_account().value());
  EXPECT_THAT(request.signing_account_delegates().value(),
              ::testing::ElementsAre("test-delegate1", "test-delegate2"));
}

TEST(PostPolicyV4EscapeTest, OnlyAscii) {
  EXPECT_EQ("\\\\\\b\\f\\n\\r\\t\\vabcd",
            *PostPolicyV4Escape("\\\b\f\n\r\t\vabcd"));
}

#if GOOGLE_CLOUD_CPP_HAVE_CODECVT && GOOGLE_CLOUD_CPP_HAVE_EXCEPTIONS
TEST(PostPolicyV4EscapeTest, InvalidUtf) {
  // In UTF8 a byte larger than 0x7f indicates that there is a byte following
  // it. Here we truncate the string to cause the UTF parser to fail.
  std::string invalid_utf8(1, 0x80);
  EXPECT_EQ(StatusCode::kInvalidArgument,
            PostPolicyV4Escape(invalid_utf8).status().code());
}
#endif  // GOOGLE_CLOUD_CPP_HAVE_CODECVT && GOOGLE_CLOUD_CPP_HAVE_EXCEPTIONS

TEST(PostPolicyV4EscapeTest, Simple) {
#if GOOGLE_CLOUD_CPP_HAVE_CODECVT && GOOGLE_CLOUD_CPP_HAVE_EXCEPTIONS
  EXPECT_EQ("\127\065abcd$", *PostPolicyV4Escape("\127\065abcd$"));
  EXPECT_EQ("\\\\\\b\\f\\n\\r\\t\\v\\u0080\\u0119",
            *PostPolicyV4Escape("\\\b\f\n\r\t\v\u0080ę"));
#else   // GOOGLE_CLOUD_CPP_HAVE_CODECVT && GOOGLE_CLOUD_CPP_HAVE_EXCEPTIONS
  EXPECT_EQ(StatusCode::kUnimplemented,
            PostPolicyV4Escape("ąę").status().code());
#endif  // GOOGLE_CLOUD_CPP_HAVE_CODECVT && GOOGLE_CLOUD_CPP_HAVE_EXCEPTIONS
}

TEST(PolicyDocumentV4Request, Printing) {
  PolicyDocumentV4 doc;
  doc.bucket = "test-bucket";
  doc.object = "test-object";
  doc.expiration = std::chrono::seconds(13);
  doc.timestamp = google::cloud::internal::ParseRfc3339("2010-06-16T11:11:11Z");
  PolicyDocumentV4Request req(doc);
  std::stringstream stream;
  stream << req;
  EXPECT_EQ(
      "PolicyDocumentRequest={{\"conditions\":[{\"key\":\"test-object\"},{\"x-"
      "goog-date\":\"20100616T111111Z\"},{\"x-goog-credential\":\"/20100616/"
      "auto/storage/"
      "goog4_request\"},{\"x-goog-algorithm\":\"GOOG4-RSA-SHA256\"}],"
      "\"expiration\":\"2010-06-16T11:11:24Z\"}}",
      stream.str());
}

}  // namespace
}  // namespace internal
}  // namespace STORAGE_CLIENT_NS
}  // namespace storage
}  // namespace cloud
}  // namespace google
