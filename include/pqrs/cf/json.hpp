#pragma once

// pqrs::cf::json v0.0

// (C) Copyright Takayama Fumihiko 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#include <pqrs/cf/array.hpp>
#include <pqrs/cf/number.hpp>
#include <pqrs/cf/set.hpp>
#include <pqrs/cf/string.hpp>
#include <pqrs/json.hpp>

namespace pqrs {
namespace cf {
inline nlohmann::json cf_type_to_json(CFTypeRef object) {
  nlohmann::json result;

  if (object) {
    CFTypeID type = CFGetTypeID(object);

    if (type == CFArrayGetTypeID()) {
      auto value = nlohmann::json::array();
      auto array = reinterpret_cast<CFArrayRef>(object);
      CFArrayApplyFunction(array,
                           {0, CFArrayGetCount(array)},
                           [](const void* o, void* context) {
                             auto j = reinterpret_cast<nlohmann::json*>(context);
                             j->push_back(cf_type_to_json(reinterpret_cast<CFTypeRef>(o)));
                           },
                           &value);
      result = nlohmann::json::object({
          {"type", "array"},
          {"value", value},
      });

    } else if (type == CFBooleanGetTypeID()) {
      result = nlohmann::json::object({
          {"type", "boolean"},
          {"value", static_cast<bool>(CFBooleanGetValue(reinterpret_cast<CFBooleanRef>(object)))},
      });

    } else if (type == CFDataGetTypeID()) {
      auto value = nlohmann::json::array();
      auto data = reinterpret_cast<CFDataRef>(object);
      auto length = CFDataGetLength(data);
      auto bytes = CFDataGetBytePtr(data);
      for (CFIndex i = 0; i < length; ++i) {
        value.push_back(bytes[i]);
      }
      result = nlohmann::json::object({
          {"type", "data"},
          {"value", value},
      });

    } else if (type == CFDictionaryGetTypeID()) {
      auto value = nlohmann::json::array();
      CFDictionaryApplyFunction(reinterpret_cast<CFDictionaryRef>(object),
                                [](const void* k, const void* v, void* context) {
                                  auto j = reinterpret_cast<nlohmann::json*>(context);
                                  j->push_back(nlohmann::json::object({
                                      {"key", cf_type_to_json(k)},
                                      {"value", cf_type_to_json(v)},
                                  }));
                                },
                                &value);
      result = nlohmann::json::object({
          {"type", "dictionary"},
          {"value", value},
      });

    } else if (type == CFNumberGetTypeID()) {
      result = nlohmann::json::object();

      auto number = reinterpret_cast<CFNumberRef>(object);
      if (CFNumberIsFloatType(number)) {
        if (auto v = make_number<double>(number)) {
          result["type"] = "number_float";
          result["value"] = *v;
        } else {
          throw json::marshal_error("failed to marshal CFNumber");
        }
      } else {
        if (auto v = make_number<int64_t>(number)) {
          result["type"] = "number_integer";
          result["value"] = *v;
        } else {
          throw json::marshal_error("failed to marshal CFNumber");
        }
      }

    } else if (type == CFSetGetTypeID()) {
      auto value = nlohmann::json::array();
      CFSetApplyFunction(reinterpret_cast<CFSetRef>(object),
                         [](const void* o, void* context) {
                           auto j = reinterpret_cast<nlohmann::json*>(context);
                           j->push_back(cf_type_to_json(reinterpret_cast<CFTypeRef>(o)));
                         },
                         &value);
      result = nlohmann::json::object({
          {"type", "set"},
          {"value", value},
      });

    } else if (type == CFStringGetTypeID()) {
      if (auto v = make_string(object)) {
        result = nlohmann::json::object({
            {"type", "string"},
            {"value", *v},
        });
      } else {
        throw json::marshal_error("failed to marshal CFString");
      }

    } else {
      throw json::marshal_error("unknown CFTypeRef");
    }
  }

  return result;
}
} // namespace cf
} // namespace pqrs
