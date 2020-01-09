#pragma once

// (C) Copyright Takayama Fumihiko 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#include <type_safe/flag_set.hpp>

namespace pqrs {
namespace cf {
namespace json {
enum class strip_option {
  none,
  collapse_dictionary,
  end_,
};
} // namespace json
} // namespace cf
} // namespace pqrs

namespace type_safe {
template <>
struct flag_set_traits<pqrs::cf::json::strip_option> : std::true_type {
  static constexpr std::size_t size() {
    return static_cast<std::size_t>(pqrs::cf::json::strip_option::end_) + 1;
  }
};
} // namespace type_safe
