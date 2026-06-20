#include <boost/ut.hpp>
#include <cstdint>
#include <fstream>
#include <pqrs/cf/json.hpp>
#include <string>

int main() {
  using namespace boost::ut;
  using namespace boost::ut::literals;

  "CFArray"_test = [] {
    {
      auto array = pqrs::cf::adopt_cf_ptr(CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks));
      CFArrayAppendValue(*array, CFSTR("example"));
      auto actual = pqrs::cf::json::to_json(*array);
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
    }

    {
      auto array = pqrs::cf::adopt_cf_ptr(CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks));
      CFArrayAppendValue(*array, kCFNull);
      expect(throws<pqrs::json::marshal_error>([&] {
        static_cast<void>(pqrs::cf::json::to_json(*array));
      }));
    }
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
    auto data = pqrs::cf::adopt_cf_ptr(CFDataCreate(kCFAllocatorDefault,
                                                    bytes,
                                                    sizeof(bytes)));
    auto actual = pqrs::cf::json::to_json(*data);
    auto expected = nlohmann::json::object({
        {"type", "data"},
        {"value", nlohmann::json::array({0, 1, 2, 3, 4, 5, 250})},
    });
    expect(actual == expected);

    auto empty_cf_data = pqrs::cf::json::to_cf_type(nlohmann::json::object({
        {"type", "data"},
        {"value", nlohmann::json::array()},
    }));
    expect(empty_cf_data);
    expect(pqrs::cf::json::to_json(*empty_cf_data) == nlohmann::json::object({
                                                          {"type", "data"},
                                                          {"value", nlohmann::json::array()},
                                                      }));
  };

  "CFDictionary"_test = [] {
    {
      auto dictionary = pqrs::cf::adopt_cf_ptr(CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                         0,
                                                                         &kCFTypeDictionaryKeyCallBacks,
                                                                         &kCFTypeDictionaryValueCallBacks));
      CFDictionaryAddValue(*dictionary, CFSTR("k"), CFSTR("v"));

      auto actual = pqrs::cf::json::to_json(*dictionary);
      auto expected = nlohmann::json::object({
          {"type", "dictionary"},
          {"value", nlohmann::json::array({nlohmann::json::object({
                        {"key", nlohmann::json::object({{"type", "string"}, {"value", "k"}})},
                        {"value", nlohmann::json::object({{"type", "string"}, {"value", "v"}})},
                    })})},
      });
      expect(actual == expected);
    }

    {
      auto dictionary = pqrs::cf::adopt_cf_ptr(CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                         0,
                                                                         &kCFTypeDictionaryKeyCallBacks,
                                                                         &kCFTypeDictionaryValueCallBacks));
      auto actual = pqrs::cf::json::to_json(*dictionary);
      auto expected = nlohmann::json::object({
          {"type", "dictionary"},
          {"value", nlohmann::json::array()},
      });
      expect(actual == expected);
    }

    {
      auto dictionary = pqrs::cf::adopt_cf_ptr(CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                         0,
                                                                         &kCFTypeDictionaryKeyCallBacks,
                                                                         &kCFTypeDictionaryValueCallBacks));
      CFDictionaryAddValue(*dictionary, CFSTR("k"), kCFNull);
      expect(throws<pqrs::json::marshal_error>([&] {
        static_cast<void>(pqrs::cf::json::to_json(*dictionary));
      }));
    }
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
    {
      auto set = pqrs::cf::adopt_cf_ptr(CFSetCreateMutable(kCFAllocatorDefault,
                                                           0,
                                                           &kCFTypeSetCallBacks));
      CFSetAddValue(*set, CFSTR("v"));

      auto actual = pqrs::cf::json::to_json(*set);
      auto expected = nlohmann::json::object({
          {"type", "set"},
          {"value", nlohmann::json::array({
                        nlohmann::json::object({
                            {"type", "string"},
                            {"value", "v"},
                        }),
                    })},
      });
      expect(actual == expected);
    }

    {
      auto set = pqrs::cf::adopt_cf_ptr(CFSetCreateMutable(kCFAllocatorDefault,
                                                           0,
                                                           &kCFTypeSetCallBacks));
      auto actual = pqrs::cf::json::to_json(*set);
      auto expected = nlohmann::json::object({
          {"type", "set"},
          {"value", nlohmann::json::array()},
      });
      expect(actual == expected);
    }

    {
      auto set = pqrs::cf::adopt_cf_ptr(CFSetCreateMutable(kCFAllocatorDefault,
                                                           0,
                                                           &kCFTypeSetCallBacks));
      CFSetAddValue(*set, kCFNull);
      expect(throws<pqrs::json::marshal_error>([&] {
        static_cast<void>(pqrs::cf::json::to_json(*set));
      }));
    }
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
        try {
          static_cast<void>(pqrs::cf::json::to_cf_type(j["input"]));
          expect(false);
        } catch (const pqrs::json::unmarshal_error& e) {
          expect(std::string(e.what()) == j["error"].get<std::string>());
        } catch (...) {
          expect(false);
        }
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

    expect(throws<pqrs::json::unmarshal_error>([] {
      static_cast<void>(pqrs::cf::json::strip_cf_type_json(nlohmann::json::object({
          {"type", "unknown"},
          {"value", nullptr},
      })));
    }));
  };

  return 0;
}
