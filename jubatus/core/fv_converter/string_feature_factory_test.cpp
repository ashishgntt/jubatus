// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <gtest/gtest.h>
#include "exception.hpp"
#include "string_feature.hpp"
#include "string_feature_factory.hpp"

namespace jubatus {
namespace core {
namespace fv_converter {

TEST(string_feature_factory, trivial) {
  string_feature_factory f;
  std::map<std::string, std::string> param;
  ASSERT_THROW(f.create("hoge", param), converter_exception);
}

TEST(string_feature_factory, dynamic) {
  string_feature_factory f;
  std::map<std::string, std::string> param;
  ASSERT_THROW(f.create("dynamic", param), converter_exception);

  param["path"] = "unknown_file_name";
  ASSERT_THROW(f.create("dynamic", param), converter_exception);

  param["path"] = LIBSPLITTER_SAMPLE;
  ASSERT_THROW(f.create("dynamic", param), converter_exception);

  param["function"] = "create";
  jubatus::util::lang::shared_ptr<string_feature> s(f.create("dynamic", param));

  std::string d("hoge fuga");
  std::vector<string_feature_element> es;
  s->extract(d, es);
  ASSERT_EQ(2u, es.size());
  ASSERT_EQ(0u, es[0].begin);
  ASSERT_EQ(4u, es[0].length);
  ASSERT_EQ("hoge", es[0].value);
  ASSERT_EQ(1.0, es[0].score);
  ASSERT_EQ(5u, es[1].begin);
  ASSERT_EQ(4u, es[1].length);
  ASSERT_EQ("fuga", es[1].value);
  ASSERT_EQ(1.0, es[1].score);
}

TEST(string_feature_factory, ngram) {
  string_feature_factory f;
  std::map<std::string, std::string> param;
  ASSERT_THROW(f.create("ngram", param), converter_exception);

  param["char_num"] = "not_a_number";
  ASSERT_THROW(f.create("ngram", param), converter_exception);

  param["char_num"] = "-1";
  ASSERT_THROW(f.create("ngram", param), converter_exception);

  param["char_num"] = "2";
  jubatus::util::lang::shared_ptr<string_feature> s(f.create("ngram", param));
}

TEST(string_feature_factory, regexp) {
  string_feature_factory f;
  std::map<std::string, std::string> param;
  ASSERT_THROW(f.create("regexp", param), converter_exception);

  param["pattern"] = "[";
  param["group"] = "1";
  ASSERT_THROW(f.create("regexp", param), converter_exception);

  param["pattern"] = "(.+)";
  param["group"] = "a";
  ASSERT_THROW(f.create("regexp", param), converter_exception);

  param["pattern"] = "(.+)";
  param["group"] = "1";
  jubatus::util::lang::shared_ptr<string_feature>(f.create("regexp", param));

  param["pattern"] = "(.+)";
  param.erase("group");
  jubatus::util::lang::shared_ptr<string_feature>(f.create("regexp", param));
}

}  // namespace fv_converter
}  // namespace core
}  // namespace jubatus
