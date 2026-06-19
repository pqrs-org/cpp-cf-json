#include <cstdint>
#include <iomanip>
#include <iostream>
#include <pqrs/cf/json.hpp>

int main() {
  std::cout << pqrs::cf::json::to_json(CFSTR("example")) << std::endl;
  if (auto number = pqrs::cf::make_cf_number(12345)) {
    std::cout << pqrs::cf::json::to_json(*number) << std::endl;
  }
  if (auto number = pqrs::cf::make_cf_number(123.45)) {
    std::cout << pqrs::cf::json::to_json(*number) << std::endl;
  }
  std::cout << pqrs::cf::json::to_json(kCFBooleanTrue) << std::endl;
  std::cout << pqrs::cf::json::to_json(kCFBooleanFalse) << std::endl;
  std::cout << pqrs::cf::json::to_json(nullptr) << std::endl;

  //
  // array
  //

  {
    auto array = pqrs::cf::adopt_cf_ptr(CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks));
    CFArrayAppendValue(*array, CFSTR("example"));
    if (auto number = pqrs::cf::make_cf_number(12345)) {
      CFArrayAppendValue(*array, *number);
    }
    CFArrayAppendValue(*array, kCFBooleanTrue);
    CFArrayAppendValue(*array, kCFBooleanFalse);

    //
    // Copy
    //

    {
      auto copy = pqrs::cf::adopt_cf_ptr(CFArrayCreateCopy(kCFAllocatorDefault, *array));
      CFArrayAppendValue(*array, *copy);
    }

    //
    // Set
    //

    {
      auto set = pqrs::cf::adopt_cf_ptr(CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks));
      CFSetAddValue(*set, CFSTR("example"));
      if (auto number = pqrs::cf::make_cf_number(12345)) {
        CFSetAddValue(*set, *number);
      }
      CFArrayAppendValue(*array, *set);
    }

    //
    // Dictionary
    //

    {
      auto dictionary = pqrs::cf::adopt_cf_ptr(CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                                         0,
                                                                         &kCFTypeDictionaryKeyCallBacks,
                                                                         &kCFTypeDictionaryValueCallBacks));
      CFDictionaryAddValue(*dictionary, CFSTR("key"), CFSTR("value"));
      if (auto number = pqrs::cf::make_cf_number(12345)) {
        CFDictionaryAddValue(*dictionary, *number, *number);
      }
      CFArrayAppendValue(*array, *dictionary);
    }

    //
    // Data
    //

    {
      uint8_t bytes[] = {0, 1, 2, 3, 4, 5, 250};
      auto data = pqrs::cf::adopt_cf_ptr(CFDataCreate(kCFAllocatorDefault,
                                                      bytes,
                                                      sizeof(bytes)));
      CFArrayAppendValue(*array, *data);
    }

    //
    // to_json
    //

    std::cout << std::setw(4) << pqrs::cf::json::to_json(*array) << std::endl;
    // std::cout << std::setw(4) << pqrs::cf::json::strip_cf_type_json(pqrs::cf::json::to_json(array), pqrs::cf::json::strip_option::none) << std::endl;
    // std::cout << std::setw(4) << pqrs::cf::json::strip_cf_type_json(pqrs::cf::json::to_json(array), pqrs::cf::json::strip_option::collapse_dictionary) << std::endl;
  }

  return 0;
}
