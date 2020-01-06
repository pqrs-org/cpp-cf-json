#include <iomanip>
#include <iostream>
#include <pqrs/cf/json.hpp>

int main(void) {
  std::cout << pqrs::cf::cf_type_to_json(CFSTR("example")) << std::endl;
  if (auto number = pqrs::cf::make_cf_number(12345)) {
    std::cout << pqrs::cf::cf_type_to_json(*number) << std::endl;
  }
  if (auto number = pqrs::cf::make_cf_number(123.45)) {
    std::cout << pqrs::cf::cf_type_to_json(*number) << std::endl;
  }
  std::cout << pqrs::cf::cf_type_to_json(kCFBooleanTrue) << std::endl;
  std::cout << pqrs::cf::cf_type_to_json(kCFBooleanFalse) << std::endl;
  std::cout << pqrs::cf::cf_type_to_json(nullptr) << std::endl;

  //
  // array
  //

  {
    auto array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    CFArrayAppendValue(array, CFSTR("example"));
    if (auto number = pqrs::cf::make_cf_number(12345)) {
      CFArrayAppendValue(array, *number);
    }
    CFArrayAppendValue(array, kCFBooleanTrue);
    CFArrayAppendValue(array, kCFBooleanFalse);

    //
    // Copy
    //

    {
      auto copy = CFArrayCreateCopy(kCFAllocatorDefault, array);
      CFArrayAppendValue(array, copy);
      CFRelease(copy);
    }

    //
    // Set
    //

    {
      auto set = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
      CFSetAddValue(set, CFSTR("example"));
      if (auto number = pqrs::cf::make_cf_number(12345)) {
        CFSetAddValue(set, *number);
      }
      CFArrayAppendValue(array, set);
      CFRelease(set);
    }

    //
    // Dictionary
    //

    {
      auto dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                  0,
                                                  &kCFTypeDictionaryKeyCallBacks,
                                                  &kCFTypeDictionaryValueCallBacks);
      CFDictionaryAddValue(dictionary, CFSTR("key"), CFSTR("value"));
      if (auto number = pqrs::cf::make_cf_number(12345)) {
        CFDictionaryAddValue(dictionary, *number, *number);
      }
      CFArrayAppendValue(array, dictionary);
      CFRelease(dictionary);
    }

    //
    // Data
    //

    {
      uint8_t bytes[] = {0, 1, 2, 3, 4, 5, 250};
      auto data = CFDataCreate(kCFAllocatorDefault,
                               bytes,
                               sizeof(bytes));
      CFArrayAppendValue(array, data);
      CFRelease(data);
    }

    //
    // cf_type_to_json
    //

    std::cout << std::setw(4) << pqrs::cf::cf_type_to_json(array) << std::endl;
    CFRelease(array);
  }

  return 0;
}
