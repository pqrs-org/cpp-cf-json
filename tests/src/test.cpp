#include <boost/ut.hpp>
#include <fstream>
#include <pqrs/cf/json.hpp>

int main(void) {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "CFArray"_test = [] {
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
    expect(actual == expected);

    CFRelease(array);
  };

  "CFBoolean"_test = [] {
    {
      auto actual = pqrs::cf::json::to_json(kCFBooleanTrue);
      auto expected = nlohmann::json::object({
          {"type", "boolean"},
          {"value", true},
      });
      expect(actual == expected);
    }

    {
      auto actual = pqrs::cf::json::to_json(kCFBooleanFalse);
      auto expected = nlohmann::json::object({
          {"type", "boolean"},
          {"value", false},
      });
      expect(actual == expected);
    }
  };

  "CFData"_test = [] {
    uint8_t bytes[] = {0, 1, 2, 3, 4, 5, 250};
    auto data = CFDataCreate(kCFAllocatorDefault,
                             bytes,
                             sizeof(bytes));
    auto actual = pqrs::cf::json::to_json(data);
    auto expected = nlohmann::json::object({
        {"type", "data"},
        {"value", nlohmann::json::array({0, 1, 2, 3, 4, 5, 250})},
    });
    expect(actual == expected);

    CFRelease(data);
  };

  "CFDictionary"_test = [] {
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
    expect(actual == expected);

    CFRelease(dictionary);
  };

  "CFNumber"_test = [] {
    {
      auto number = pqrs::cf::make_cf_number(123);
      auto actual = pqrs::cf::json::to_json(*number);
      auto expected = nlohmann::json::object({
          {"type", "number_integer"},
          {"value", 123},
      });
      expect(actual == expected);
    }

    {
      auto number = pqrs::cf::make_cf_number(123.5);
      auto actual = pqrs::cf::json::to_json(*number);
      auto expected = nlohmann::json::object({
          {"type", "number_float"},
          {"value", 123.5},
      });
      expect(actual == expected);
    }
  };

  "CFSet"_test = [] {
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
    expect(actual == expected);

    CFRelease(dictionary);
  };

  "CFString"_test = [] {
    auto actual = pqrs::cf::json::to_json(CFSTR("example"));
    auto expected = nlohmann::json::object({
        {"type", "string"},
        {"value", "example"},
    });
    expect(actual == expected);
  };

  "to_cf_type"_test = [] {
    {
      std::ifstream s("data/valid.json");
      auto json = nlohmann::json::parse(s);
      auto object = pqrs::cf::json::to_cf_type(json);
      expect(object);
      auto actual = pqrs::cf::json::to_json(*object);
      expect(actual == json);
    }

    {
      std::ifstream s("data/errors.json");
      auto json = nlohmann::json::parse(s);
      for (const auto& j : json) {
        expect(throws<pqrs::json::unmarshal_error>([j] {
          pqrs::cf::json::to_cf_type(j["input"]);
        }));
        expect(throws<pqrs::json::unmarshal_error>([j] {
          pqrs::cf::json::to_cf_type(j["input"]);
        }));
      }
    }
  };

  "strip_cf_type_json"_test = [] {
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

      expect(actual == expected);
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

      expect(actual == expected);
    }
  };

  return 0;
}
