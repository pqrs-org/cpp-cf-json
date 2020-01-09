#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <pqrs/cf/json.hpp>

TEST_CASE("CFArray") {
  auto array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
  CFArrayAppendValue(array, CFSTR("example"));
  auto actual = pqrs::cf::json::to_json(array);
  auto expected = nlohmann::json::object({
      {"type", "array"},
      {"value", nlohmann::json::array({
                    nlohmann::json::object({
                        {"type", "string"},
                        {"value", "example"},
                    }),
                })},
  });
  REQUIRE(actual == expected);

  CFRelease(array);
}

TEST_CASE("CFBoolean") {
  {
    auto actual = pqrs::cf::json::to_json(kCFBooleanTrue);
    auto expected = nlohmann::json::object({
        {"type", "boolean"},
        {"value", true},
    });
    REQUIRE(actual == expected);
  }

  {
    auto actual = pqrs::cf::json::to_json(kCFBooleanFalse);
    auto expected = nlohmann::json::object({
        {"type", "boolean"},
        {"value", false},
    });
    REQUIRE(actual == expected);
  }
}

TEST_CASE("CFData") {
  uint8_t bytes[] = {0, 1, 2, 3, 4, 5, 250};
  auto data = CFDataCreate(kCFAllocatorDefault,
                           bytes,
                           sizeof(bytes));
  auto actual = pqrs::cf::json::to_json(data);
  auto expected = nlohmann::json::object({
      {"type", "data"},
      {"value", nlohmann::json::array({0, 1, 2, 3, 4, 5, 250})},
  });
  REQUIRE(actual == expected);

  CFRelease(data);
}

TEST_CASE("CFDictionary") {
  auto dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                              0,
                                              &kCFTypeDictionaryKeyCallBacks,
                                              &kCFTypeDictionaryValueCallBacks);
  CFDictionaryAddValue(dictionary, CFSTR("k"), CFSTR("v"));

  auto actual = pqrs::cf::json::to_json(dictionary);
  auto expected = nlohmann::json::object({
      {"type", "dictionary"},
      {"value", nlohmann::json::array({nlohmann::json::object({
                    {"key", nlohmann::json::object({{"type", "string"}, {"value", "k"}})},
                    {"value", nlohmann::json::object({{"type", "string"}, {"value", "v"}})},
                })})},
  });
  REQUIRE(actual == expected);

  CFRelease(dictionary);
}

TEST_CASE("CFNumber") {
  {
    auto number = pqrs::cf::make_cf_number(123);
    auto actual = pqrs::cf::json::to_json(*number);
    auto expected = nlohmann::json::object({
        {"type", "number_integer"},
        {"value", 123},
    });
    REQUIRE(actual == expected);
  }

  {
    auto number = pqrs::cf::make_cf_number(123.5);
    auto actual = pqrs::cf::json::to_json(*number);
    auto expected = nlohmann::json::object({
        {"type", "number_float"},
        {"value", 123.5},
    });
    REQUIRE(actual == expected);
  }
}

TEST_CASE("CFSet") {
  auto dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                              0,
                                              &kCFTypeDictionaryKeyCallBacks,
                                              &kCFTypeDictionaryValueCallBacks);
  CFDictionaryAddValue(dictionary, CFSTR("k"), CFSTR("v"));

  auto actual = pqrs::cf::json::to_json(dictionary);
  auto expected = nlohmann::json::object({
      {"type", "dictionary"},
      {"value", nlohmann::json::array({nlohmann::json::object({
                    {"key", nlohmann::json::object({{"type", "string"}, {"value", "k"}})},
                    {"value", nlohmann::json::object({{"type", "string"}, {"value", "v"}})},
                })})},
  });
  REQUIRE(actual == expected);

  CFRelease(dictionary);
}

TEST_CASE("CFString") {
  auto actual = pqrs::cf::json::to_json(CFSTR("example"));
  auto expected = nlohmann::json::object({
      {"type", "string"},
      {"value", "example"},
  });
  REQUIRE(actual == expected);
}

TEST_CASE("to_cf_type") {
  {
    std::ifstream s("data/valid.json");
    auto json = nlohmann::json::parse(s);
    auto object = pqrs::cf::json::to_cf_type(json);
    REQUIRE(object);
    auto actual = pqrs::cf::json::to_json(*object);
    REQUIRE(actual == json);
  }

  {
    std::ifstream s("data/errors.json");
    auto json = nlohmann::json::parse(s);
    for (const auto& j : json) {
      REQUIRE_THROWS_AS(
          pqrs::cf::json::to_cf_type(j["input"]),
          pqrs::json::unmarshal_error);
      REQUIRE_THROWS_WITH(
          pqrs::cf::json::to_cf_type(j["input"]),
          j["error"]);
    }
  }
}

TEST_CASE("strip_cf_type_json") {
  {
    nlohmann::json actual;
    nlohmann::json expected;

    {
      std::ifstream s("data/valid.json");
      auto json = nlohmann::json::parse(s);
      actual = pqrs::cf::json::strip_cf_type_json(json);
    }
    {
      std::ifstream s("data/stripped.json");
      expected = nlohmann::json::parse(s);
    }

    REQUIRE(actual == expected);
  }

  {
    nlohmann::json actual;
    nlohmann::json expected;

    {
      std::ifstream s("data/valid.json");
      auto json = nlohmann::json::parse(s);
      actual = pqrs::cf::json::strip_cf_type_json(json, pqrs::cf::json::strip_option::collapse_dictionary);
    }
    {
      std::ifstream s("data/stripped_collapse_dictionary.json");
      expected = nlohmann::json::parse(s);
    }

    REQUIRE(actual == expected);
  }
}
