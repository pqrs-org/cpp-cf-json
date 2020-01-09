[![Build Status](https://github.com/pqrs-org/cpp-cf-json/workflows/CI/badge.svg)](https://github.com/pqrs-org/cpp-cf-json/actions)
[![License](https://img.shields.io/badge/license-Boost%20Software%20License-blue.svg)](https://github.com/pqrs-org/cpp-cf-json/blob/master/LICENSE.md)

# cpp-cf-json

JSON serialization and deserialization for `CFTypeRef`.

## Requirements

cpp-cf-json depends the following classes.

- [pqrs::cf::array](https://github.com/pqrs-org/cpp-cf-array).
- [pqrs::cf::dictionary](https://github.com/pqrs-org/cpp-cf-dictionary).
- [pqrs::cf::number](https://github.com/pqrs-org/cpp-cf-number).
- [pqrs::cf::set](https://github.com/pqrs-org/cpp-cf-set).
- [pqrs::cf::string](https://github.com/pqrs-org/cpp-cf-string).
- [pqrs::json](https://github.com/pqrs-org/cpp-json).
- [type_safe](https://github.com/foonathan/type_safe)

## Install

### Using package manager

You can install `include/pqrs` by using [cget](https://github.com/pfultz2/cget).

```shell
cget install pqrs-org/cget-recipes
cget install pqrs-org/cpp-cf-json
```

### Manual install

Copy `include/pqrs` directory into your include directory.
