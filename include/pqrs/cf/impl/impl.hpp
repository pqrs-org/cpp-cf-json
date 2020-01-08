#pragma once

// (C) Copyright Takayama Fumihiko 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#include <pqrs/json.hpp>

namespace pqrs {
namespace cf {
namespace json {
namespace impl {
std::string get_type(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_object()) {
    throw pqrs::json::unmarshal_error("json must be object, but is `"s + json.dump() + "`"s);
  }

  auto it = json.find("type");
  if (it == std::end(json)) {
    throw pqrs::json::unmarshal_error("`type` is not found in `"s + json.dump() + "`"s);
  }

  auto type_json = it.value();
  if (!type_json.is_string()) {
    throw pqrs::json::unmarshal_error("`type` must be string, but is `"s + type_json.dump() + "`"s);
  }

  return type_json.get<std::string>();
}

nlohmann::json get_value_json(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_object()) {
    throw pqrs::json::unmarshal_error("json must be object, but is `"s + json.dump() + "`"s);
  }

  auto it = json.find("value");
  if (it == std::end(json)) {
    throw pqrs::json::unmarshal_error("`value` is not found in `"s + json.dump() + "`"s);
  }
  return it.value();
}

void validate_array_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_array()) {
    throw pqrs::json::unmarshal_error("`value` must be array, but is `"s + json.dump() + "`"s);
  }
}

void validate_boolean_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_boolean()) {
    throw pqrs::json::unmarshal_error("`value` must be boolean, but is `"s + json.dump() + "`"s);
  }
}

void validate_data_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_array()) {
    throw pqrs::json::unmarshal_error("`value` must be array, but is `"s + json.dump() + "`"s);
  }
}

void validate_dictionary_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_array()) {
    throw pqrs::json::unmarshal_error("`value` must be array, but is `"s + json.dump() + "`"s);
  }
}

nlohmann::json get_dictionary_key(const nlohmann::json& json) {
  using namespace std::string_literals;

  auto it = json.find("key");
  if (it == std::end(json)) {
    throw pqrs::json::unmarshal_error("`key` is not found in `"s + json.dump() + "`"s);
  }

  return it.value();
}

nlohmann::json get_dictionary_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  auto it = json.find("value");
  if (it == std::end(json)) {
    throw pqrs::json::unmarshal_error("`value` is not found in `"s + json.dump() + "`"s);
  }

  return it.value();
}

void validate_number_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_number()) {
    throw pqrs::json::unmarshal_error("`value` must be number, but is `"s + json.dump() + "`"s);
  }
}

void validate_set_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_array()) {
    throw pqrs::json::unmarshal_error("`value` must be array, but is `"s + json.dump() + "`"s);
  }
}

void validate_string_value(const nlohmann::json& json) {
  using namespace std::string_literals;

  if (!json.is_string()) {
    throw pqrs::json::unmarshal_error("`value` must be string, but is `"s + json.dump() + "`"s);
  }
}
} // namespace impl
} // namespace json
} // namespace cf
} // namespace pqrs
