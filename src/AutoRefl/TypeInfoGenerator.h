//
// Created by Admin on 26/03/2025.
//

#pragma once

#include <MySRefl/config.h>
#include "Meta.h"

namespace My::MySRefl {
class TypeInfoGenerator {
 public:
  enum class ConstMode { Constepxr, Const, NonConst };

  struct Config {
    bool nonNamespaceNameWithoutQuotation{false};
    bool namespaceNameWithQuotation{false};
    bool isAttrValueToFunction{false};

    std::string_view nameof_namespace = _MySRefl::nameof_namespace;

    bool isInitializerAsAttr{true};

    std::string_view nameof_initializer = _MySRefl::nameof_initializer;
    bool isInitializerToFunction{true};

    std::string_view nameof_constructor = _MySRefl::nameof_constructor;
    std::string_view nameof_destructor = _MySRefl::nameof_destructor;

    bool generateDefaultFunctions{true};
    std::string_view nameof_default_functions =
        _MySRefl::nameof_default_functions;
    ConstMode attrListConstMode{ConstMode::Constepxr};
    ConstMode fieldListConstMode{ConstMode::Constepxr};
  };

  TypeInfoGenerator(Config config = Config{}) : config{std::move(config)} {}

  std::string Generate(const std::vector<TypeMeta>& typeMetas);

 private:
  Config config;
};
}  // namespace My::MySRefl
